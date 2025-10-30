from contextlib import closing
import pandas as pd
import psycopg2 as pg
from pathlib import Path
import re
from termcolor import cprint
import time
from typing import List, Optional, final

from tasks.rcss2d import FieldSide, UniformNumber
from tasks.v1.types import *
from .utils import MatchData

async def update_match_playertypes_at_postgres(playertypes_filepath: Path, conn, schema: str) -> None:
    profiling_start = time.time()
    print(f"Starting file {str(playertypes_filepath)[:100]}...")
    
    match_data = MatchData.from_filepath(playertypes_filepath)
    table = None
    try:
        table = pd.read_csv(
            playertypes_filepath, 
            compression=('gzip' if playertypes_filepath.match('*.gz') else None),
        )
    except Exception as excpt:
        print(excpt)
        print(f"Failed to read table data from {playertypes_filepath}")
        return

    cursor = conn.cursor()

    # Find match in the database
    try:
        matchid_subquery = cursor.mogrify("SELECT match_id FROM public.matches WHERE match_timestamp = %s;", (match_data.timestamp,)).decode('utf8')
        cursor.execute(matchid_subquery)
        # Cache the result for later use
        (match_id_cache,) = cursor.fetchone()
        #
        # 2. Add all player types
        #
        playertypes_updating_columns = [
            'match_id_fk',
            'id',
            'player_decay',
            'inertia_moment',
            'dash_power_rate',
            'kickable_margin',
            'kick_rand',
            'extra_stamina',
            'effort_min',
            'effort_max'
        ]
        playertypes_updating_columns_str = ','.join(playertypes_updating_columns)
        playertypes_update_statement = ",".join(
            f"{column} = new.{column}" for column in playertypes_updating_columns[2:] # Skip match_id_fk and id
        )
        values = ",".join(
            cursor.mogrify("(%s,%s,%s,%s,%s,%s,%s,%s,%s,%s)", (
                match_id_cache,
                *(csv_row[playertypes_column] for playertypes_column in playertypes_updating_columns[1:]),
            )).decode('utf8') for _, csv_row in table.iterrows()
        )
        query = f"UPDATE \"{schema}\".playertypes AS current SET " +\
                playertypes_update_statement +\
                " from (values " +\
                values +\
                f") AS new({playertypes_updating_columns_str}) " +\
                f"WHERE current.match_id_fk = new.match_id_fk AND current.id = new.id;"
        cursor.execute(query)
    except Exception as excpt:
        print(excpt)
        dumpfilename = 'v1data_update_match_playertypes_at_postgres.errlog'
        with closing(open(dumpfilename,'a')) as logfile:
            logfile.write("\n".join([
                f"playertypes_filepath = {playertypes_filepath}",
                f"match_data = {match_data}",
                f"match_id_cache = {match_id_cache}",
                str(excpt)
            ]))
        print(f"The transaction failed for the file {playertypes_filepath}\nRollback and abort badly.\nCache is dumped to {dumpfilename}")
        conn.rollback()
    finally:
        cursor.close()
    conn.commit()

    profiling_end = time.time()
    print(f"Finished match {match_data.timestamp} in {profiling_end-profiling_start} sec")

async def copy_match_contents_to_postgres(match_filepaths: List[Path], conn, schema: str) -> None:
    profiling_start = time.time()
    print(f"Starting file group {str(match_filepaths)[:100]}...")
    class Tables:
        def __init__(self) -> None:
            self.match:         Optional[pd.DataFrame] = None
            self.playertypes:   Optional[pd.DataFrame] = None
            self.dash:          Optional[pd.DataFrame] = None
            self.turn:          Optional[pd.DataFrame] = None
            self.kick:          Optional[pd.DataFrame] = None
            self.tackle:        Optional[pd.DataFrame] = None
    
    tables = Tables()
    match_data = None

    for table_path in match_filepaths:
        tabletype = TableType.from_filepath(table_path)
        try:
            df = pd.read_csv(
                table_path, 
                compression=('gzip' if table_path.match('*.gz') else None),
            )
            if tabletype is TableType.DASH:
                if tables.dash is not None:
                    print('Duplicated Dash tables in the match group. Abort safely.')
                    return
                tables.dash = df
            elif tabletype is TableType.TURN:
                if tables.turn is not None:
                    print('Duplicated Turn tables in the match group. Abort safely.')
                    return
                tables.turn = df
            elif tabletype is TableType.KICK:
                if tables.kick is not None:
                    print('Duplicated Kick tables in the match group. Abort safely.')
                    return
                tables.kick = df
            elif tabletype is TableType.TACKLE:
                if tables.tackle is not None:
                    print('Duplicated Tackle tables in the match group. Abort safely.')
                    return
                tables.tackle = df
            elif tabletype is TableType.MATCH:
                if tables.match is not None:
                    print('Duplicated Match tables in the match group. Abort safely.')
                    return
                tables.match = df
                match_data = MatchData.from_filepath(table_path)
            elif tabletype is TableType.PTYPES:
                if tables.playertypes is not None:
                    print('Duplicated PlayerTypes tables in the match group. Abort safely.')
                    return
                tables.playertypes = df
            else:
                ## Not interested
                pass
        except Exception as excpt:
            cprint(excpt)
            return
    
    if (tables.match is None
        or tables.playertypes is None
        or tables.dash is None
        or tables.turn is None
        or tables.kick is None
        or tables.tackle is None):
        raise ValueError(f'Filepath group {match_filepaths} is incomplete. Abort safely.')
    
    if (match_data.timestamp is None
        or match_data.left_teamname is None
        or match_data.left_finalscore is None
        or match_data.right_teamname is None
        or match_data.right_finalscore is None):
        raise ValueError(f'Filepath group {match_filepaths} is incomplete. Abort.')

    with closing(conn.cursor()) as cursor:
        cursor: pg.cursor
        #
        # Caches. Pre-declare them to be able to dump its contents in case of a failure
        #
        match_id_cache = None
        playertype_id_cache = {}            # cache key is (typeid)
        matchstate_id_cache = []            # cache key is (csv_row_index) 
        matchstate_id_cache_per_time = {}   # cache key is (cycle, stopped_) 
        playerstate_id_cache = {}           # cache key is (matchstate_id, teamname, unum) 
        playercommand_id_dash_cache = {}    # cache key is (cycle, stopped_cycle, teamname, unum)
        playercommand_id_turn_cache = {}    # cache key is (cycle, stopped_cycle, teamname, unum)
        playercommand_id_kick_cache = {}    # cache key is (cycle, stopped_cycle, teamname, unum)
        playercommand_id_tackle_cache = {}  # cache key is (cycle, stopped_cycle, teamname, unum)
        try:
            #
            # 1. Create subquery to fetch this match's match_id
            #
            matchid_subquery = cursor.mogrify("SELECT match_id FROM public.matches WHERE match_timestamp = %s;", (match_data.timestamp,)).decode('utf8')
            cursor.execute(matchid_subquery)
            # Cache the result for later use
            (match_id_cache,) = cursor.fetchone()
            #
            # 2. Add all player types
            #
            playertypes_columns = [
                'match_id_fk',
                'id',
                'player_decay',
                'inertia_moment',
                'dash_power_rate',
                'kickable_margin',
                'kick_rand',
                'extra_stamina',
                'effort_min',
                'effort_max'
            ]
            playertypes_columns_str = ",".join(playertypes_columns)
            values = ",".join(
                cursor.mogrify("(%s,%s,%s,%s,%s,%s,%s,%s,%s,%s)", (
                    match_id_cache,
                    *(csv_row[playertypes_column] for playertypes_column in playertypes_columns[1:]),
                )).decode('utf8') for _, csv_row in tables.playertypes.iterrows()
            )
            query = f"INSERT INTO {schema}.playertypes ({playertypes_columns_str}) VALUES {values} RETURNING playertype_id;"
            cursor.execute(query)
            #
            # Cache playertype_id return for later use
            #
            for typeid, returned in enumerate(cursor.fetchall()):
                (playertype_id,) = returned
                playertype_id_cache[typeid] = playertype_id
            #
            # 3. Add all match states
            #
            matchstates_columns = [
                'match_id_fk',
                'cycle',
                'stopped_cycle',
                'playmode',
                'left_teamname',
                'right_teamname',
                'ball_x',
                'ball_y',
                'ball_vx',
                'ball_vy'
            ]
            matchstates_columns_str = ",".join(matchstates_columns)
            match_columns = [
                ' cycle',
                ' stopped',
                ' playmode',
                ' l_name',
                ' r_name',
                ' b_x',
                ' b_y',
                ' b_vx',
                ' b_vy'
            ]
            values = ",".join(
                cursor.mogrify("(%s,%s,%s,%s,%s,%s,%s,%s,%s,%s)", (
                    match_id_cache,
                    *(csv_row[match_column] for match_column in match_columns),
                )).decode('utf8') for _, csv_row in tables.match.iterrows()
            )
            query = f"INSERT INTO {schema}.matchstates ({matchstates_columns_str}) VALUES {values} RETURNING matchstate_id, cycle, stopped_cycle;"
            cursor.execute(query)
            #
            # Cache matchstate_id return for later use
            #
            for (matchstate_id, cycle, stopped_cycle) in cursor.fetchall():
                matchstate_id_cache.append(matchstate_id)
                matchstate_id_cache_per_time[(cycle, stopped_cycle)] = matchstate_id
            #
            # 4. Add all player states
            #
            playerstates_columns = [
                'match_id_fk',
                'matchstate_id_fk',
                'playertype_id_fk',
                'teamname',
                'unum',
                'isgoalie',
                'isdiscarded',
                'x',
                'y',
                'vx',
                'vy',
                'body',
                'stamina',
                'stamina_capacity'
            ]
            playerstates_columns_str = ",".join(playerstates_columns)
            values = ",".join(
                cursor.mogrify("(%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s)", (
                    match_id_cache,
                    matchstate_id_cache[index],
                    playertype_id_cache[csv_row[' %s%s_t' % (side,unum)]],
                    (match_data.left_teamname if side == 'l' else match_data.right_teamname),
                    unum,
                    str(csv_row[' %s%s_goalie' % (side,unum)]), # Postgres accepts '1'/'0' as true/false
                    str(csv_row[' %s%s_discarded' % (side,unum)]), # Postgres accepts '1'/'0' as true/false
                    csv_row[' %s%s_x' % (side,unum)],
                    csv_row[' %s%s_y' % (side,unum)],
                    csv_row[' %s%s_vx' % (side,unum)],
                    csv_row[' %s%s_vy' % (side,unum)],
                    csv_row[' %s%s_body' % (side,unum)],
                    csv_row[' %s%s_stamina' % (side,unum)],
                    csv_row[' %s%s_stamina_cap' % (side,unum)],
                )).decode('utf8') for index, csv_row in tables.match.iterrows() for side in ('l', 'r') for unum in range(1,12)
            )
            query = f"INSERT INTO {schema}.playerstates ({playerstates_columns_str}) VALUES {values} RETURNING playerstate_id, matchstate_id_fk, teamname, unum;"
            cursor.execute(query)
            #
            # Cache playerstate_id return for later use
            #
            for (playerstate_id, matchstate_id, teamname, unum) in cursor.fetchall():
                playerstate_id_cache[(matchstate_id, teamname, unum)] = playerstate_id
            playerstate_id_cache_usedkeys = set() # This is important to counter flaws in the data where multiple commands have been issued by the same player at a single moment of time
            playerstate_id_cache_usedkeys_accumulated = set() # Same here, but to counter different kinds of commands issued by the same player at a single moment of time
            def valid_playercommand_csv_row(playercommand_csv_row) -> bool:
                if playercommand_csv_row['running_time'] in (0, 3000):
                    ## Due to a flaw in our v1 dataset. We don't have the state of cycles 0 and 3000
                    return False
                playerstate_id_cache_key = (matchstate_id_cache_per_time[(playercommand_csv_row['running_time'],playercommand_csv_row['stopped_time'])], playercommand_csv_row['teamname'], playercommand_csv_row['unum'])
                if (playerstate_id_cache_key in playerstate_id_cache_usedkeys 
                or playerstate_id_cache_key in playerstate_id_cache_usedkeys_accumulated):
                    # This playerstate has already been linked to a previous command
                    return False
                else:
                    # This playerstate is available, reserve it for this command
                    playerstate_id_cache_usedkeys.add(playerstate_id_cache_key)
                return True
            #
            #  Some common stuff
            #
            playercommands_columns = [
                'matchstate_id_fk',
                'cycle_fk',
                'stopped_cycle_fk',
                'unum_fk', 
                'teamname_fk',
                'playerstate_id_fk',
                'playercommand_type'
            ]
            playercommands_columns_str = ",".join(playercommands_columns)

            #
            #   NOTE:   We have made a mistake when writing the rcl2csv program so we don't have the original order of issuing
            #       of commands when a dumb player sends multiple commands at the same server cycle.
            #           The server executes commands as they come, therefore we have lost the information of the true command executed in
            #       a situation like this. Luckly, this typically happens very little in a match. In order to not lose the cycle information,
            #       we pretend the server has a priority for commands: tackle, kick, turn, dash. We choose this to prioritize less issued commands.
            #           Therefore, do not change the order we process the tables for a given match.
            #

            #
            # 5. Add all tackles
            #
            playerstate_id_cache_usedkeys.clear() ## Clear as we go through every row once again
            values = ",".join(
                cursor.mogrify("(%s,%s,%s,%s,%s,%s,%s)", (
                    matchstate_id_cache_per_time[(csv_row['running_time'],csv_row['stopped_time'])],
                    csv_row['running_time'],
                    csv_row['stopped_time'],
                    csv_row['unum'],
                    csv_row['teamname'],
                    playerstate_id_cache[(matchstate_id_cache_per_time[(csv_row['running_time'],csv_row['stopped_time'])], csv_row['teamname'], csv_row['unum'])],
                    'tackle'
                )).decode('utf8') for _, csv_row in tables.tackle.iterrows() if valid_playercommand_csv_row(csv_row)
            )
            query = f"INSERT INTO {schema}.playercommands ({playercommands_columns_str}) VALUES {values} RETURNING playercommand_id, cycle_fk, stopped_cycle_fk, teamname_fk, unum_fk"
            cursor.execute(query)
            #
            # Capture command ids and use them to send tackle
            # Use (cycle_fk, stopped_cycle_fk, teamname_fk, unum_fk) as cache key
            #
            for (playercommand_id, cycle_fk, stopped_cycle_fk, teamname_fk, unum_fk) in cursor.fetchall():
                playercommand_id_tackle_cache[(cycle_fk, stopped_cycle_fk, teamname_fk, unum_fk)] = playercommand_id
            # Now add tackle polymorphic columns
            tackle_columns = ['tackle_id', 'tackle_direction']
            tackle_columns_str = ",".join(tackle_columns)
            playerstate_id_cache_usedkeys.clear() ## Clear as we go through every row once again
            values = ",".join(
                cursor.mogrify("(%s,%s)", (
                    playercommand_id_tackle_cache[(csv_row['running_time'],csv_row['stopped_time'],csv_row['teamname'],csv_row['unum'])],
                    csv_row['tackle_direction']
                )).decode('utf8') for _, csv_row in tables.tackle.iterrows() if valid_playercommand_csv_row(csv_row)
            )
            query = f"INSERT INTO {schema}.tackle_commands ({tackle_columns_str}) VALUES {values};"
            cursor.execute(query)


            #
            # 6. Add all kicks
            #
            playerstate_id_cache_usedkeys_accumulated.update(playerstate_id_cache_usedkeys)
            playerstate_id_cache_usedkeys.clear() ## Clear as we go through every row once again
            values = ",".join(
                cursor.mogrify("(%s,%s,%s,%s,%s,%s,%s)", (
                    matchstate_id_cache_per_time[(csv_row['running_time'],csv_row['stopped_time'])],
                    csv_row['running_time'],
                    csv_row['stopped_time'],
                    csv_row['unum'],
                    csv_row['teamname'],
                    playerstate_id_cache[(matchstate_id_cache_per_time[(csv_row['running_time'],csv_row['stopped_time'])], csv_row['teamname'], csv_row['unum'])],
                    'kick'
                )).decode('utf8') for _, csv_row in tables.kick.iterrows() if valid_playercommand_csv_row(csv_row)
            )
            query = f"INSERT INTO {schema}.playercommands ({playercommands_columns_str}) VALUES {values} RETURNING playercommand_id, cycle_fk, stopped_cycle_fk, teamname_fk, unum_fk"
            cursor.execute(query)
            #
            # Capture command ids and use them to send kick
            # Use (cycle_fk, stopped_cycle_fk, teamname_fk, unum_fk) as cache key
            #
            for (playercommand_id, cycle_fk, stopped_cycle_fk, teamname_fk, unum_fk) in cursor.fetchall():
                playercommand_id_kick_cache[(cycle_fk, stopped_cycle_fk, teamname_fk, unum_fk)] = playercommand_id
            # Now add turn polymorphic columns
            kick_columns = ['kick_id', 'kick_power', 'kick_direction']
            kick_columns_str = ",".join(kick_columns)
            playerstate_id_cache_usedkeys.clear() ## Clear as we go through every row once again
            values = ",".join(
                cursor.mogrify("(%s,%s,%s)", (
                    playercommand_id_kick_cache[(csv_row['running_time'],csv_row['stopped_time'],csv_row['teamname'],csv_row['unum'])],
                    csv_row['kick_power'],
                    csv_row['kick_direction']
                )).decode('utf8') for _, csv_row in tables.kick.iterrows() if valid_playercommand_csv_row(csv_row)
            )
            query = f"INSERT INTO {schema}.kick_commands ({kick_columns_str}) VALUES {values};"
            cursor.execute(query)


            #
            # 7. Add all turns
            #
            playerstate_id_cache_usedkeys_accumulated.update(playerstate_id_cache_usedkeys) 
            playerstate_id_cache_usedkeys.clear() ## Clear as we go through every row once again
            values = ",".join(
                cursor.mogrify("(%s,%s,%s,%s,%s,%s,%s)", (
                    matchstate_id_cache_per_time[(csv_row['running_time'],csv_row['stopped_time'])],
                    csv_row['running_time'],
                    csv_row['stopped_time'],
                    csv_row['unum'],
                    csv_row['teamname'],
                    playerstate_id_cache[(matchstate_id_cache_per_time[(csv_row['running_time'],csv_row['stopped_time'])], csv_row['teamname'], csv_row['unum'])],
                    'turn'
                )).decode('utf8') for _, csv_row in tables.turn.iterrows() if valid_playercommand_csv_row(csv_row)
            )
            query = f"INSERT INTO {schema}.playercommands ({playercommands_columns_str}) VALUES {values} RETURNING playercommand_id, cycle_fk, stopped_cycle_fk, teamname_fk, unum_fk"
            cursor.execute(query)
            #
            # Capture command ids and use them to send turn
            # Use (cycle_fk, stopped_cycle_fk, teamname_fk, unum_fk) as cache key
            #
            for (playercommand_id, cycle_fk, stopped_cycle_fk, teamname_fk, unum_fk) in cursor.fetchall():
                playercommand_id_turn_cache[(cycle_fk, stopped_cycle_fk, teamname_fk, unum_fk)] = playercommand_id
            # Now add turn polymorphic columns
            turn_columns = ['turn_id', 'turn_moment']
            turn_columns_str = ",".join(turn_columns)
            playerstate_id_cache_usedkeys.clear() ## Clear as we go through every row once again
            values = ",".join(
                cursor.mogrify("(%s,%s)", (
                    playercommand_id_turn_cache[(csv_row['running_time'],csv_row['stopped_time'],csv_row['teamname'],csv_row['unum'])],
                    csv_row['turn_moment']
                )).decode('utf8') for _, csv_row in tables.turn.iterrows() if valid_playercommand_csv_row(csv_row)
            )
            query = f"INSERT INTO {schema}.turn_commands ({turn_columns_str}) VALUES {values};"
            cursor.execute(query)


            #
            # 8. Add all dashes
            #
            # Add base table playercommands
            playerstate_id_cache_usedkeys_accumulated.update(playerstate_id_cache_usedkeys)
            playerstate_id_cache_usedkeys.clear()
            values = ",".join(
                cursor.mogrify("(%s,%s,%s,%s,%s,%s,%s)", (
                    matchstate_id_cache_per_time[(csv_row['running_time'],csv_row['stopped_time'])],
                    csv_row['running_time'],
                    csv_row['stopped_time'],
                    csv_row['unum'],
                    csv_row['teamname'],
                    playerstate_id_cache[(matchstate_id_cache_per_time[(csv_row['running_time'],csv_row['stopped_time'])], csv_row['teamname'], csv_row['unum'])],
                    'dash'
                )).decode('utf8') for _, csv_row in tables.dash.iterrows() if valid_playercommand_csv_row(csv_row)
            )
            query = f"INSERT INTO {schema}.playercommands ({playercommands_columns_str}) VALUES {values} RETURNING playercommand_id, cycle_fk, stopped_cycle_fk, teamname_fk, unum_fk;"
            cursor.execute(query)
            #
            # Capture command ids and use them to send dash
            # Use (cycle_fk, stopped_cycle_fk, teamname_fk, unum_fk) as cache key
            #
            for (playercommand_id, cycle_fk, stopped_cycle_fk, teamname_fk, unum_fk) in cursor.fetchall():
                playercommand_id_dash_cache[(cycle_fk, stopped_cycle_fk, teamname_fk, unum_fk)] = playercommand_id
            # Now add dash polymorphic columns
            dash_columns = ['dash_id', 'dash_power', 'dash_direction']
            dash_columns_str = ",".join(dash_columns)
            playerstate_id_cache_usedkeys.clear() ## Clear as we go through every row once again
            values = ",".join(
                cursor.mogrify("(%s,%s,%s)", (
                    playercommand_id_dash_cache[(csv_row['running_time'],csv_row['stopped_time'],csv_row['teamname'],csv_row['unum'])],
                    csv_row['dash_power'],
                    csv_row['dash_direction']
                )).decode('utf8') for _, csv_row in tables.dash.iterrows() if valid_playercommand_csv_row(csv_row)
            )
            query = f"INSERT INTO {schema}.dash_commands ({dash_columns_str}) VALUES {values};"
            cursor.execute(query)

        except Exception as excpt:
            print(excpt)
            dumpfilename = 'v1data_copy_match_contents_to_postgres.errlog'
            with closing(open(dumpfilename,'a')) as logfile:
                logfile.write("\n".join([
                    f"match_filepaths = {match_filepaths}",
                    f"match_data = {match_data}",
                    f"match_id_cache = {match_id_cache}",
                    f"playertype_id_cache = {playertype_id_cache}",
                    f"matchstate_id_cache = {matchstate_id_cache}",
                    f"matchstate_id_cache_per_time = {matchstate_id_cache_per_time}",
                    f"playerstate_id_cache = {playerstate_id_cache}",
                    f"playercommand_id_dash_cache = {playercommand_id_dash_cache}",
                    f"playercommand_id_turn_cache = {playercommand_id_turn_cache}",
                    f"playercommand_id_kick_cache = {playercommand_id_kick_cache}",
                    f"playercommand_id_tackle_cache = {playercommand_id_tackle_cache}",
                    str(excpt)
                ]))
            print(f"The transaction failed for the group {match_filepaths}\nRollback and abort badly.\nCache is dumped to {dumpfilename}")
            conn.rollback()
            return
        conn.commit()
        profiling_end = time.time()
        print(f"Finished match {match_data.timestamp} in {profiling_end-profiling_start} sec")
        

async def normalize_raw_features(filepath: Path, compress: bool, output_dir: Path) -> None:
    print(f"Start file {filepath}")
    start_time = time.time()
    ## Analize table type
    tabletype = TableType.from_filepath(filepath)
    ## Set columns to be normalized
    normalizable = None
    if tabletype is TableType.DASH:
        normalizable = [ DashColumn.DASH_POWER, DashColumn.DASH_DIRECTION ]
        output_suffix = '.dash.csv'
    elif tabletype is TableType.TURN:
        normalizable = [ TurnColumn.TURN_MOMENT]
        output_suffix = '.turn.csv'
    elif tabletype is TableType.KICK:
        normalizable = [ KickColumn.KICK_POWER, KickColumn.KICK_DIRECTION ]
        output_suffix = '.kick.csv'
    elif tabletype is TableType.TACKLE:
        normalizable = [ TackleColumn.TACKLE_DIRECTION ]
        output_suffix = '.tackle.csv'
    elif tabletype is TableType.MATCH:
        normalizable = [ 
            MatchGeneralColumn.BALL_X,
            MatchGeneralColumn.BALL_Y,
            MatchGeneralColumn.BALL_VX,
            MatchGeneralColumn.BALL_VY
        ]
        player_normalizable = [
            SinglePlayerColumn.X,
            SinglePlayerColumn.Y,
            SinglePlayerColumn.VX,
            SinglePlayerColumn.VY,
            SinglePlayerColumn.BODY_ANGLE,
            SinglePlayerColumn.STAMINA,
            SinglePlayerColumn.STAMINA_RESERVE
        ]
        for side in [ FieldSide.LEFT, FieldSide.RIGHT ]:
            for unum in range(1,12):
                for player_column in player_normalizable:
                    normalizable.append(MatchPlayerColumn(side, UniformNumber.from_int(unum), player_column))
        output_suffix = '.match.csv'
    elif tabletype is TableType.PTYPES:
        normalizable = [ 
            PlayerTypesColumn.DASH_POWER_RATE,
            PlayerTypesColumn.PLAYER_DECAY,
            PlayerTypesColumn.INERTIA_MOMENT,
            PlayerTypesColumn.KICKABLE_MARGIN,
            PlayerTypesColumn.KICK_RAND,
            PlayerTypesColumn.EXTRA_STAMINA,
            PlayerTypesColumn.EFFORT_MIN,
            PlayerTypesColumn.EFFORT_MAX
        ]
        output_suffix = '.playertypes.csv'
    else:
        ## Not interested
        return
    ## Create normalizers
    normalizers = [ normalizer_from_columntype(column_type) for column_type in normalizable ]
    ## Read table
    df = None
    columns_names = list(map(str, normalizable))
    try:
        df = pd.read_csv(
            filepath, 
            compression=('gzip' if filepath.match('*.gz') else None),
        )
    except Exception as excpt:
        print(excpt)
        return
    ## Normalize
    for column, normalizer in zip(columns_names, normalizers):
        df[column] = normalizer.normalize(df[column].astype('float64').values)
    ## Dump output table
    outputfilename = filepath.name.split('.')[0] + output_suffix + ('.gz' if compress else '')
    df.to_csv(
        output_dir / outputfilename, 
        compression=('gzip' if compress else None),
        index=False
    )
    print(f"Finished file {filepath} in {time.time() - start_time} sec")


async def extract_raw_features(filepath: Path, compress: bool, output_dir: Path) -> None:
    ## Analyze table type
    raw_feature_list = None
    output_suffix = ''
    try:
        tabletype = TableType.from_filepath(filepath)
    except ValueError as excpt:
        print(excpt.message)
        print("Skip file.")
        return
    if tabletype == TableType.DASH:
        raw_feature_list = ['running_time', 'stopped_time', 'teamname', 'unum', 'dash_power', 'dash_direction']
        output_suffix += '.dash.csv'
    elif tabletype == TableType.KICK:
        raw_feature_list = ['running_time', 'stopped_time', 'teamname', 'unum', 'kick_power', 'kick_direction']
        output_suffix += '.kick.csv'
    elif tabletype == TableType.TURN:
        raw_feature_list = ['running_time', 'stopped_time', 'teamname', 'unum', 'turn_moment']
        output_suffix += '.turn.csv'
    elif tabletype == TableType.TACKLE:
        raw_feature_list = ['running_time', 'stopped_time', 'teamname', 'unum', 'tackle_direction']
        output_suffix += '.tackle.csv'
    elif tabletype == TableType.MATCH:
        raw_feature_list = [
            'cycle', 
            'stopped', 
            'playmode', 
            'l_name', 
            'r_name',
            'b_x', 
            'b_y',
            'b_vx',
            'b_vy',
            *[ f"{side}{i}_{player_feature}"
                for side in ['l', 'r'] 
                    for i in range(1,12) 
                        for player_feature in [
                            't',            # To extract heterogeneous types
                            'goalie',       # To identify the goalie
                            'discarded',    # To clean data of frames with invalid players
                            'x',
                            'y',
                            'vx',
                            'vy',
                            'body',
                            'stamina',
                            'stamina_cap'
                        ] 
            ]
        ]
        output_suffix += '.match.csv'
        ## FIXME: The line below won't be needed once we have version 1.0.1 of the dataset
        raw_feature_list = [f' {raw_feature_name}' for raw_feature_name in raw_feature_list]
        ##
    elif tabletype == TableType.PTYPES:
        raw_feature_list = [
            'id',
            'dash_power_rate',
            'player_decay',
            'inertia_moment',
            'kickable_margin',
            'kick_rand',
            'extra_stamina',
            'effort_min',
            'effort_max'
        ]
        output_suffix += '.playertypes.csv'
    else:
        # Skip this CSV, not of our interest
        return
    ## Load table only with the data needed
    df = None
    try:
        df = pd.read_csv(
            filepath, 
            compression=('gzip' if filepath.match('*.gz') else None),
            usecols=raw_feature_list
        )
    except Exception as excpt:
        cprint(excpt)
        return
    ## Dump output table
    outputfilename = filepath.name.split('.')[0] + output_suffix + ('.gz' if compress else '')
    df.to_csv(
        output_dir / outputfilename, 
        compression=('gzip' if compress else None),
        index=False
    )


async def copy_match_metadata_to_postgres(match_filepath: Path, connection) -> None:
    profiling_start = time.time()
    print(f"Starting file {str(match_filepath)[:100]}...")

    match_data = MatchData.from_filepath(match_filepath)
    if (None in (
        match_data.timestamp, 
        match_data.left_teamname, 
        match_data.left_finalscore, 
        match_data.right_teamname, 
        match_data.right_finalscore
    )):
        raise ValueError(f'Match filepath {match_filepath} is incomplete. Abort.')
        
    
    cursor: pg.cursor = connection.cursor()
    try:
        columns_str = ','.join([
            'match_timestamp',
            'left_finalteamname',
            'right_finalteamname',
            'left_finalscore',
            'right_finalscore'
        ])
        values_str = cursor.mogrify("(%s,%s,%s,%s,%s)", (
            match_data.timestamp, 
            match_data.left_teamname,
            match_data.right_teamname,
            match_data.left_finalscore,
            match_data.right_finalscore
        )).decode('utf8')
        query = f"INSERT INTO public.matches ({columns_str}) VALUES {values_str}"
        cursor.execute(query)
        # print(query)
    except Exception as excpt:
        cprint(excpt)
        print(f"The transaction failed for the match file {match_filepath}\nRollback and abort.")
        connection.rollback()
        return
    finally:
        cursor.close()
    connection.commit()
    profiling_end = time.time()
    print(f"Finished match {match_data.timestamp} in {profiling_end-profiling_start} sec")

