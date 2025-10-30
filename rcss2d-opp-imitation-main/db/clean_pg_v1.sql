DELETE FROM extra.matchstates WHERE playmode != 'play_on';

DELETE FROM extra.playercommands WHERE teamname_fk != 'HELIOS2019';

DELETE FROM extra.playercommands WHERE unum_fk = 1;
