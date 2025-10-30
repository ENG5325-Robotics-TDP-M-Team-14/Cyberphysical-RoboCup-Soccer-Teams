from enum import Enum
from pathlib import Path
from typing import Union

from tasks.rcss2d import *

class TableType(Enum):

    DASH    = 'dash'
    TURN    = 'turn'
    KICK    = 'kick'
    TACKLE  = 'tackle'
    MATCH   = 'match'
    PTYPES  = 'playertypes'
    PPARAMS = 'playerparams'
    SPARAMS = 'serverparams'

    def __str__(self) -> str:
        return self.value

    @staticmethod
    def from_filepath(tablepath: Path) -> 'TableType':
        if tablepath.name.endswith('.dash.csv') or tablepath.name.endswith('.dash.csv.gz'):
            return TableType.DASH
        if tablepath.name.endswith('.kick.csv') or tablepath.name.endswith('.kick.csv.gz'):
            return TableType.KICK
        if tablepath.name.endswith('.turn.csv') or tablepath.name.endswith('.turn.csv.gz'):
            return TableType.TURN
        if tablepath.name.endswith('.tackle.csv') or tablepath.name.endswith('.tackle.csv.gz'):
            return TableType.TACKLE
        if tablepath.name.endswith('.match.csv') or tablepath.name.endswith('.match.csv.gz'):
            return TableType.MATCH
        if tablepath.name.endswith('.playertypes.csv') or tablepath.name.endswith('.playertypes.csv.gz'):
            return TableType.PTYPES
        if tablepath.name.endswith('.playerparams.csv') or tablepath.name.endswith('.playerparams.csv.gz'):
            return TableType.PPARAMS
        if tablepath.name.endswith('.serverparams.csv') or tablepath.name.endswith('.serverparams.csv.gz'):
            return TableType.SPARAMS
        raise ValueError(f"Unsupported table type of CSV file {tablepath}")

class TableColumnType:
    """ Base class for all table columns specifications. """
    ROWNUM = '#'

class CommandTableColumn(TableColumnType):
    """ Base class with all command tables columns specifications. """
    RUNNING_TIME        = 'running_time'
    STOPPED_TIME        = 'stopped_time'
    GLOBAL_ORDER_INDEX  = 'global_command_order'
    TEAMNAME            = 'teamname'
    UNIFORM_NUMBER      = 'unum'

class DashColumn(CommandTableColumn, Enum):
    """ The set of columns available at a Dash table. """
    DASH_POWER          = 'dash_power'
    DASH_DIRECTION      = 'dash_direction'
    def __str__(self) -> str:
        return self.value

class TurnColumn(CommandTableColumn, Enum):
    """ The set of columns available at a Turn table. """
    TURN_MOMENT         = 'turn_moment'
    def __str__(self) -> str:
        return self.value

class KickColumn(CommandTableColumn, Enum):
    """ The set of columns available at a Kick table. """
    KICK_POWER          = 'kick_power'
    KICK_DIRECTION      = 'kick_direction'
    def __str__(self) -> str:
        return self.value

class TackleColumn(CommandTableColumn, Enum):
    """ The set of columns available at a Tackle table. """
    TACKLE_DIRECTION    = 'tackle_direction'
    FOUL_INTENTION      = 'foul_intention'
    def __str__(self) -> str:
        return self.value

class ServerParamsColumn(TableColumnType, Enum):
    """ The set of columns available at a ServerParams table. """
    # TODO
    def __str__(self) -> str:
        return self.value

class PlayerParamsColumn(TableColumnType, Enum):
    """ The set of columns available at a PlayerParams table. """
    # TODO
    def __str__(self) -> str:
        return self.value

class PlayerTypesColumn(TableColumnType, Enum):
    """ The set of columns available at a PlayerTypes table. """
    ID                              = 'id'
    PLAYER_SPEED_MAX                = 'player_speed_max'
    STAMINA_INC_MAX                 = 'stamina_inc_max'
    PLAYER_DECAY                    = 'player_decay'
    INERTIA_MOMENT                  = 'inertia_moment'
    DASH_POWER_RATE                 = 'dash_power_rate'
    PLAYER_SIZE                     = 'player_size'
    KICKABLE_MARGIN                 = 'kickable_margin'
    KICK_RAND                       = 'kick_rand'
    EXTRA_STAMINA                   = 'extra_stamina'
    EFFORT_MAX                      = 'effort_max'
    EFFORT_MIN                      = 'effort_min'
    KICK_POWER_RATE                 = 'kick_power_rate'
    FOUL_DETECT_PROBABILITY         = 'foul_detect_probability'
    CATCHABLE_AREA_LENGTH_STRETCH   = 'catchable_area_l_stretch'
    def __str__(self) -> str:
        return self.value

class MatchGeneralColumn(TableColumnType, Enum):
    """ The set of columns available at a Match table that are not related to a specific player. """
    CYCLE               = ' cycle'
    STOPPED             = ' stopped'
    PLAYMODE            = ' playmode'
    LEFT_NAME           = ' l_name'
    LEFT_SCORE          = ' l_score'
    LEFT_PENALTY_SCORE  = ' l_pen_score'
    RIGHT_NAME          = ' r_name'
    RIGHT_SCORE         = ' l_score'
    RIGHT_PENALTY_SCORE = ' l_pen_score'
    BALL_X              = ' b_x'
    BALL_Y              = ' b_y'
    BALL_VX             = ' b_vx'
    BALL_VY             = ' b_vy'
    def __str__(self) -> str:
        return self.value

"""
 There are too many columns relating to players, so we have an enum just to the columns of a single player.
"""
class SinglePlayerColumn(Enum):
    """ The set of columns available at a Match table that related to a specific player. """
    TYPE                    = 't'
    KICK_TRIED              = 'kick_tried'
    KICK_FAILED             = 'kick_failed'
    IS_GOALIE               = 'goalie'
    CATCH_TRIED             = 'catch_tried'
    CATCH_FAILED            = 'catch_failed'
    DISCARDED               = 'discarded'
    COLLIDED_WITH_BALL      = 'collided_with_ball'
    COLLIDED_WITH_PLAYER    = 'collided_with_player'
    TACKLE_TRIED            = 'tackle_tried'
    TACKLE_FAILED           = 'tackle_failed'
    BACKPASSED              = 'backpassed'
    FREEKICKED_WRONG        = 'freekicked_wrong'
    COLLIDED_WITH_POST      = 'collided_with_post'
    FOUL_FROZEN             = 'foul_frozen'
    YELLOW_CARD             = 'yellow_card'
    RED_CARD                = 'red_card'
    DEFENDED_ILLEGALY       = 'defended_illegaly'
    X                       = 'x'
    Y                       = 'y'
    VX                      = 'vx'
    VY                      = 'vy'
    BODY_ANGLE              = 'body'
    NECK_ANGLE              = 'neck'
    ARM_POINT_X             = 'arm_point_x'
    ARM_POINT_Y             = 'arm_point_y'
    VIEW_QUALITY            = 'view_quality'
    VIEW_WIDTH              = 'view_width'
    STAMINA                 = 'stamina'
    EFFORT                  = 'effort'
    STAMINA_RECOVERY        = 'stamina_rec'
    STAMINA_RESERVE         = 'stamina_cap'
    FOCUS_SIDE              = 'focus_side'
    FOCUS_UNIFORM           = 'focus_unum'
    KICK_COUNT              = 'kick_count'
    DASH_COUNT              = 'dash_count'
    TURN_COUNT              = 'turn_count'
    CATCH_COUNT             = 'catch_count'
    MOVE_COUNT              = 'move_count'
    TURN_NECK_COUNT         = 'turnneck_count'
    CHANGE_VIEW_COUNT       = 'changeview_count'
    SAY_COUNT               = 'say_count'
    TACKLE_COUNT            = 'tackle_count'
    POINTTO_COUNT           = 'arm_count'
    FOCUS_COUNT             = 'focus_count'
    def __str__(self) -> str:
        return self.value


class MatchPlayerColumn:
    """ A player-related column of the Match table. """

    def __init__(self, side: FieldSide, uniform: UniformNumber, column: SinglePlayerColumn) -> None:
        self.side = side
        self.uniform = uniform
        self.column = column
    
    def __str__(self) -> str:
        return MatchPlayerColumn.name(self.side, self.uniform, self.column)

    @staticmethod
    def name(side: FieldSide, uniform: UniformNumber, column: SinglePlayerColumn) -> str:
        return f' {side}{uniform}_{column}' # The whitespace is on purpose because rcg2csv mistakenly is putting whitespaces in column names

StandardTableColumn = Union[
    DashColumn, 
    TurnColumn, 
    KickColumn, 
    TackleColumn, 
    ServerParamsColumn, 
    PlayerParamsColumn, 
    PlayerTypesColumn, 
    MatchGeneralColumn
]
TableColumn = Union[StandardTableColumn, MatchPlayerColumn]

def normalizer_from_columntype(column_type: TableColumn) -> FeatureNormalizer:
    if type(column_type) is MatchPlayerColumn:
        column_type : MatchPlayerColumn
        if column_type.column is SinglePlayerColumn.X:
            return XCoordNormalizerV16()
        if column_type.column is SinglePlayerColumn.Y:
            return YCoordNormalizerV16()
        if column_type.column is SinglePlayerColumn.VX or column_type.column is SinglePlayerColumn.VY:
            return PlayerSpeedNormalizerV16()
        if column_type.column is SinglePlayerColumn.BODY_ANGLE:
            return AngleDegreeNormalizerV16()
        if column_type.column is SinglePlayerColumn.STAMINA:
            return PlayerStaminaNormalizerV16()
        if column_type.column is SinglePlayerColumn.STAMINA_RESERVE:
            return PlayerStaminaResNormalizerV16()
        raise ValueError(f'No Normalizer for Player Column {column_type.column}')
    column_type : StandardTableColumn
    ## Commands
    if column_type is MatchGeneralColumn.BALL_X:
        return XCoordNormalizerV16()
    if column_type is MatchGeneralColumn.BALL_Y:
        return YCoordNormalizerV16()
    if column_type is MatchGeneralColumn.BALL_VX or column_type is MatchGeneralColumn.BALL_VY:
        return BallSpeedNormalizerV16()
    if column_type is DashColumn.DASH_POWER:
        return CommandPowerNormalizerV16()
    if column_type is DashColumn.DASH_DIRECTION:
        return AngleDegreeNormalizerV16()
    if column_type is TurnColumn.TURN_MOMENT:
        return AngleDegreeNormalizerV16()
    if column_type is KickColumn.KICK_POWER:
        return CommandPowerNormalizerV16()
    if column_type is KickColumn.KICK_DIRECTION:
        return AngleDegreeNormalizerV16()
    if column_type is TackleColumn.TACKLE_DIRECTION:
        return AngleDegreeNormalizerV16()
    ## Player Types
    if column_type is PlayerTypesColumn.DASH_POWER_RATE:
        return DashPowerRateNormalizerV16()
    if column_type is PlayerTypesColumn.STAMINA_INC_MAX:
        return StaminaIncMaxNormalizerV16()
    if column_type is PlayerTypesColumn.PLAYER_DECAY:
        return PlayerDecayNormalizerV16()
    if column_type is PlayerTypesColumn.INERTIA_MOMENT:
        return InertiaMomentNormalizerV16()
    if column_type is PlayerTypesColumn.KICKABLE_MARGIN:
        return KickableMarginNormalizerV16()
    if column_type is PlayerTypesColumn.KICK_RAND:
        return KickRandNormalizerV16()
    if column_type is PlayerTypesColumn.EXTRA_STAMINA:
        return ExtraStaminaNormalizerV16()
    if column_type is PlayerTypesColumn.EFFORT_MIN:
        return EffortMinNormalizerV16()
    if column_type is PlayerTypesColumn.EFFORT_MAX:
        return EffortMaxNormalizerV16()
    raise TypeError(f'Unsupported TableColumn of type {type(column_type)}')
