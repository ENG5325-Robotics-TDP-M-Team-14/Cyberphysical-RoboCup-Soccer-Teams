BEGIN;

CREATE SCHEMA IF NOT EXISTS ; -- PUT YOUR SCHEMA NAME HERE! e.g. data
SET SCHEMA ; -- PUT YOUR SCHEMA NAME HERE! e.g. 'data'

CREATE TABLE IF NOT EXISTS playertypes (
    playertype_id int PRIMARY KEY GENERATED ALWAYS AS IDENTITY,
    match_id_fk             int REFERENCES public.matches ON DELETE CASCADE ON UPDATE RESTRICT NOT NULL,
    id                      int NOT NULL CHECK(id >= 0),
    player_decay            numeric NOT NULL,
    inertia_moment          numeric NOT NULL,
    dash_power_rate         numeric NOT NULL,
    kickable_margin         numeric NOT NULL,
    kick_rand               numeric NOT NULL,
    extra_stamina           numeric NOT NULL,
    effort_min              numeric NOT NULL,
    effort_max              numeric NOT NULL,
    UNIQUE (playertype_id, match_id_fk),
    UNIQUE (match_id_fk, id)
);

CREATE TABLE IF NOT EXISTS matchstates (
    matchstate_id int PRIMARY KEY GENERATED ALWAYS AS IDENTITY,
    match_id_fk             int REFERENCES public.matches ON DELETE CASCADE ON UPDATE RESTRICT NOT NULL,
    cycle                   int NOT NULL CHECK(cycle > 0),
    stopped_cycle           int NOT NULL CHECK(stopped_cycle >= 0),
    playmode                public.playmode_enum NOT NULL,
    left_teamname           varchar(32) CHECK (left_teamname IS NULL OR length(left_teamname) > 0),
    right_teamname          varchar(32) CHECK (right_teamname IS NULL OR length(right_teamname) > 0),
    ball_x                  numeric NOT NULL,
    ball_y                  numeric NOT NULL,
    ball_vx                 numeric NOT NULL,
    ball_vy                 numeric NOT NULL,
    CONSTRAINT teams_have_different_names CHECK (left_teamname != right_teamname),
    UNIQUE (matchstate_id, match_id_fk),
    UNIQUE (matchstate_id, cycle, stopped_cycle),
    UNIQUE (match_id_fk, cycle, stopped_cycle)
);

CREATE TABLE IF NOT EXISTS playerstates (
    playerstate_id          int PRIMARY KEY GENERATED ALWAYS AS IDENTITY,
    match_id_fk             int NOT NULL, -- Redundant, but necessary to code FK integrity
    matchstate_id_fk        int NOT NULL,
    playertype_id_fk        int NOT NULL,
    teamname                varchar(32) NOT NULL CHECK (teamname IS NULL OR length(teamname) > 0),
    unum                    int NOT NULL CHECK (unum BETWEEN 1 AND 11),
    isgoalie                boolean,
    isdiscarded             boolean,
    x                       numeric,
    y                       numeric,
    vx                      numeric,
    vy                      numeric,
    body                    numeric,
    stamina                 numeric,
    stamina_capacity        numeric,
    UNIQUE (playerstate_id, teamname, unum),
    UNIQUE (matchstate_id_fk, teamname, unum),
    FOREIGN KEY (match_id_fk)                   REFERENCES public.matches(match_id) ON DELETE CASCADE ON UPDATE RESTRICT,
    FOREIGN KEY (matchstate_id_fk, match_id_fk) REFERENCES matchstates(matchstate_id, match_id_fk) ON DELETE CASCADE ON UPDATE RESTRICT,
    FOREIGN KEY (playertype_id_fk, match_id_fk) REFERENCES playertypes(playertype_id, match_id_fk) ON DELETE CASCADE ON UPDATE RESTRICT
);

CREATE TABLE IF NOT EXISTS playercommands (
    playercommand_id    int PRIMARY KEY GENERATED ALWAYS AS IDENTITY,
    matchstate_id_fk    int NOT NULL,
    cycle_fk            int NOT NULL,
    stopped_cycle_fk    int NOT NULL,
    unum_fk             int NOT NULL, 
    teamname_fk         varchar(32) NOT NULL,
    playerstate_id_fk   int NOT NULL,
    playercommand_type  public.playercommand_type_enum,
    UNIQUE (playerstate_id_fk),
    UNIQUE (matchstate_id_fk, playerstate_id_fk),
    UNIQUE (matchstate_id_fk, teamname_fk, unum_fk),
    UNIQUE (cycle_fk, stopped_cycle_fk, playerstate_id_fk),
    FOREIGN KEY (matchstate_id_fk, cycle_fk, stopped_cycle_fk)	REFERENCES matchstates(matchstate_id, cycle, stopped_cycle) ON DELETE CASCADE ON UPDATE CASCADE,
    FOREIGN KEY (playerstate_id_fk, teamname_fk, unum_fk)		REFERENCES playerstates(playerstate_id, teamname, unum) ON DELETE CASCADE ON UPDATE CASCADE
);

CREATE TABLE IF NOT EXISTS dash_commands (
    dash_id                 int REFERENCES playercommands ON DELETE CASCADE ON UPDATE RESTRICT PRIMARY KEY,
    dash_power              numeric NOT NULL,
    dash_direction          numeric NOT NULL DEFAULT 0
);

CREATE TABLE IF NOT EXISTS turn_commands (
    turn_id                 int REFERENCES playercommands ON DELETE CASCADE ON UPDATE RESTRICT PRIMARY KEY,
    turn_moment             numeric NOT NULL
);

CREATE TABLE IF NOT EXISTS kick_commands (
    kick_id                 int REFERENCES playercommands ON DELETE CASCADE ON UPDATE RESTRICT PRIMARY KEY,
    kick_power              numeric NOT NULL,
    kick_direction          numeric NOT NULL
);

CREATE TABLE IF NOT EXISTS tackle_commands (
    tackle_id               int REFERENCES playercommands ON DELETE CASCADE ON UPDATE RESTRICT PRIMARY KEY,
    tackle_direction        numeric NOT NULL
);

COMMIT;
