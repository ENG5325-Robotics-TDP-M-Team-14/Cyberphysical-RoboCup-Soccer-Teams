# Role FSM specification (2D + 3D)

Canonical project entrypoint: [README.md](../../README.md). This page is the detailed architecture/reference document for role logic only.

This document formalizes current role behavior from code into explicit FSM/logic/policy for:
- Striker
- Defender
- Goalkeeper
- Role allocation (static and dynamic)

It is intentionally code-faithful, not aspirational.

## 2D simulator (rcssserver + sample_player)

### Role allocation rule (2D)

Static allocation in 4-player mode:
- `unum=1` is launched with `-g` (goalkeeper), players `2..4` are field players:
  - `environment/2d-environment/starter-stack/Agent/src/start-4players.sh:293`
  - `environment/2d-environment/starter-stack/Agent/src/start-4players.sh:311`
- Default launcher uses `number=4`:
  - `environment/2d-environment/starter-stack/Agent/src/start-4players.sh:36`
- Home-role slots in 4-player shape are by `unum`:
  - `1=goalie`, `2=defender`, `3/4=midfielder-forward lanes`
  - `environment/2d-environment/starter-stack/Agent/src/bhv_basic_move.cpp:173`
  - `environment/2d-environment/starter-stack/Agent/src/bhv_basic_move.cpp:180`
  - `environment/2d-environment/starter-stack/Agent/src/bhv_basic_move.cpp:183`
  - `environment/2d-environment/starter-stack/Agent/src/bhv_basic_move.cpp:186`

Dynamic overrides:
- In `PlayOn`, any non-goalie can temporarily become the ball-winner via intercept predicate:
  - `environment/2d-environment/starter-stack/Agent/src/bhv_basic_move.cpp:94`
- In set plays, kicker is dynamic: non-goalie with home position nearest to ball:
  - `environment/2d-environment/starter-stack/Agent/src/bhv_set_play.cpp:519`
  - `environment/2d-environment/starter-stack/Agent/src/bhv_set_play.cpp:543`
  - `environment/2d-environment/starter-stack/Agent/src/bhv_set_play.cpp:555`
- Goal kick special case: goalkeeper is forced kicker:
  - `environment/2d-environment/starter-stack/Agent/src/bhv_set_play.cpp:532`

### Striker FSM (2D)

Definition used here:
- "Striker" = attacking field player (typically `unum 3/4`) or whichever field player currently controls/contests the ball.

Core entry point:
- `environment/2d-environment/starter-stack/Agent/src/sample_player.cpp:300`

States:
1. `PREPROCESS`
2. `ATTACK_WITH_BALL`
3. `CHASE_OR_SUPPORT`
4. `SETPLAY_KICKER`

Transitions:
- `ANY -> PREPROCESS` each cycle, if preprocess consumes cycle then stop:
  - `environment/2d-environment/starter-stack/Agent/src/sample_player.cpp:306`
- `PREPROCESS -> ATTACK_WITH_BALL` when `PlayOn`, non-goalie, `kickable==true` after teammate check:
  - `environment/2d-environment/starter-stack/Agent/src/sample_player.cpp:313`
  - `environment/2d-environment/starter-stack/Agent/src/sample_player.cpp:341`
  - `environment/2d-environment/starter-stack/Agent/src/sample_player.cpp:349`
- `PREPROCESS -> CHASE_OR_SUPPORT` when `PlayOn`, non-goalie, not kickable:
  - `environment/2d-environment/starter-stack/Agent/src/sample_player.cpp:355`
- `ANY -> SETPLAY_KICKER` in non-`PlayOn`, own set play and `is_kicker()`:
  - `environment/2d-environment/starter-stack/Agent/src/bhv_set_play.cpp:93`
  - `environment/2d-environment/starter-stack/Agent/src/bhv_set_play.cpp:94`

Action policy in `ATTACK_WITH_BALL` (`Bhv_BasicOffensiveKick`):
1. Shoot if ball is inside configured shoot range:
   - `environment/2d-environment/starter-stack/Agent/src/bhv_basic_offensive_kick.cpp:106`
   - `environment/2d-environment/starter-stack/Agent/src/bhv_basic_offensive_kick.cpp:111`
2. Else pass if nearest opponent is within 10m and pass lane exists:
   - `environment/2d-environment/starter-stack/Agent/src/bhv_basic_offensive_kick.cpp:83`
   - `environment/2d-environment/starter-stack/Agent/src/bhv_basic_offensive_kick.cpp:124`
3. Else dribble if forward sector is clear:
   - `environment/2d-environment/starter-stack/Agent/src/bhv_basic_offensive_kick.cpp:154`
4. Else hold if pressure is low (`nearest_opp_dist > 2.5`):
   - `environment/2d-environment/starter-stack/Agent/src/bhv_basic_offensive_kick.cpp:94`
5. Else clear ball:
   - `environment/2d-environment/starter-stack/Agent/src/bhv_basic_offensive_kick.cpp:168`

Action policy in `CHASE_OR_SUPPORT` (`Bhv_BasicMove`):
- Intercept if:
  - no kickable teammate, and
  - `self_min <= press_threshold` OR (`self_min <= mate_min` and `self_min < opp_min + press_threshold`)
  - `environment/2d-environment/starter-stack/Agent/src/bhv_basic_move.cpp:94`
- Else move to role home position projected by ball plus offside clamp:
  - `environment/2d-environment/starter-stack/Agent/src/bhv_basic_move.cpp:108`
  - `environment/2d-environment/starter-stack/Agent/src/bhv_basic_move.cpp:132`
  - `environment/2d-environment/starter-stack/Agent/src/bhv_basic_move.cpp:258`

### Defender FSM (2D)

Definition used here:
- "Defender" = `unum 2` in 4-player template, with dynamic interception identical to other field players.

States:
1. `PREPROCESS`
2. `DEFENSIVE_INTERCEPT`
3. `SHAPE_HOLD`
4. `ON_BALL_CLEAR_OR_PASS`
5. `SETPLAY_MARK_OR_KICK`

Transitions:
- `PREPROCESS -> DEFENSIVE_INTERCEPT` on same `Bhv_BasicMove` intercept predicate:
  - `environment/2d-environment/starter-stack/Agent/src/bhv_basic_move.cpp:94`
- `PREPROCESS -> SHAPE_HOLD` otherwise:
  - `environment/2d-environment/starter-stack/Agent/src/bhv_basic_move.cpp:108`
- `PREPROCESS -> ON_BALL_CLEAR_OR_PASS` when defender becomes kickable in `PlayOn`:
  - `environment/2d-environment/starter-stack/Agent/src/sample_player.cpp:349`
- `ANY -> SETPLAY_MARK_OR_KICK` in set plays (dynamic kicker check):
  - `environment/2d-environment/starter-stack/Agent/src/bhv_set_play.cpp:93`
  - `environment/2d-environment/starter-stack/Agent/src/bhv_set_play.cpp:94`

Policy notes:
- Defender uses same offensive kick stack when kickable (no separate defender-only kick policy).
- Defensive identity is mostly from formation slot and lower x-range, not a separate defender class.

### Goalkeeper FSM (2D)

Core entry:
- `environment/2d-environment/starter-stack/Agent/src/sample_player.cpp:314`

States:
1. `PREPROCESS`
2. `CATCH`
3. `DISTRIBUTE` (kick logic)
4. `GOAL_LINE_POSITION`
5. `SETPLAY_GK`

Transitions in `PlayOn`:
- `PREPROCESS -> CATCH` when catch-ban expired, ball within catch area, and ball in own penalty rect:
  - `environment/2d-environment/starter-stack/Agent/src/sample_player.cpp:324`
- `PREPROCESS -> DISTRIBUTE` when kickable:
  - `environment/2d-environment/starter-stack/Agent/src/sample_player.cpp:331`
- `PREPROCESS -> GOAL_LINE_POSITION` otherwise:
  - `environment/2d-environment/starter-stack/Agent/src/sample_player.cpp:337`

`GOAL_LINE_POSITION` policy (`Bhv_GoalieBasicMove`):
- Tackle first if feasible:
  - `environment/2d-environment/starter-stack/Agent/src/bhv_goalie_basic_move.cpp:79`
- Else intercept if goalie reaches ball before both teammate and opponent:
  - `environment/2d-environment/starter-stack/Agent/src/bhv_goalie_basic_move.cpp:86`
- Else move to target point on goal line computed from ball geometry/prediction:
  - `environment/2d-environment/starter-stack/Agent/src/bhv_goalie_basic_move.cpp:90`
  - `environment/2d-environment/starter-stack/Agent/src/bhv_goalie_basic_move.cpp:101`

Set-play goalkeeper behavior:
- Goalie free kick behavior except back-pass/indirect free kick modes:
  - `environment/2d-environment/starter-stack/Agent/src/bhv_set_play.cpp:82`

## 3D simulator (SimSpark + FCPCodebase)

### Role allocation rule (3D)

Static priors:
- `unum=1` uses goalkeeper robot type in normal `Agent`:
  - `environment/3d-environment/FCPCodebase/agent/Agent.py:57`
- Formation home positions are defined by `formation_id` + `unum`:
  - `environment/3d-environment/FCPCodebase/world/commons/formations.py:1`

Dynamic allocation each cycle:
- Compute teammate distances to predicted slow-ball position, discarding stale/fallen teammates (distance forced to 1000):
  - `environment/3d-environment/FCPCodebase/agent/Agent.py:204`
  - `environment/3d-environment/FCPCodebase/agent/Agent.py:205`
- `active_player_unum = argmin(teammates_ball_sq_dist) + 1`:
  - `environment/3d-environment/FCPCodebase/agent/Agent.py:219`
- If `active_player_unum == self.unum`: role is active striker controller:
  - `environment/3d-environment/FCPCodebase/agent/Agent.py:251`
- Else if `self.unum == 1`: goalkeeper holding policy:
  - `environment/3d-environment/FCPCodebase/agent/Agent.py:242`
- Else: defender/support policy:
  - `environment/3d-environment/FCPCodebase/agent/Agent.py:245`

### Striker FSM (3D)

Explicit controller class:
- `environment/3d-environment/FCPCodebase/agent/Agent.py:10`

Named states:
- `SEARCH`, `APPROACH`, `ALIGN`, `KICK`, `RECOVER`
  - `environment/3d-environment/FCPCodebase/agent/Agent.py:11`

Transitions/policy:
1. `-> RECOVER` if goalkeeper is active during opponent-kick play-mode group:
   - `environment/3d-environment/FCPCodebase/agent/Agent.py:25`
   - action: move at init position facing ball.
2. `-> KICK` on our corner kick:
   - `environment/3d-environment/FCPCodebase/agent/Agent.py:28`
   - action: directional kick into front-goal space.
3. `-> APPROACH` (defensive press) when opponent is considerably closer:
   - predicate: `min_opp + press_margin < min_team`
   - `environment/3d-environment/FCPCodebase/agent/Agent.py:32`
   - action: move between ball and own goal.
4. If currently in internal kick execution (`agent.state == 2`) and press-trigger occurs:
   - `-> RECOVER` and request kick abort
   - `environment/3d-environment/FCPCodebase/agent/Agent.py:33`
5. Otherwise attacking branch:
   - choose shot target:
     - direct goal if `dist(ball, (15.05,0)) <= goal_shot_dist_thresh_m`
     - else alternate direction toward `(7.5,0)`
     - `environment/3d-environment/FCPCodebase/agent/Agent.py:40`
   - state assignment:
     - `KICK` if `agent.state == 2`
     - else `ALIGN` if `ball_dist <= 0.7`
     - else `APPROACH`
     - `environment/3d-environment/FCPCodebase/agent/Agent.py:44`
   - execute `kick(...)` and latch internal `agent.state`:
     - `environment/3d-environment/FCPCodebase/agent/Agent.py:45`

Pass assist command:
- pass command is enabled in play-on and central/attacking x (`ball_x < 6`):
  - `environment/3d-environment/FCPCodebase/agent/Agent.py:23`

### Defender FSM (3D)

Definition used here:
- "Defender" = non-goalie, non-active teammate in normal mode.

States:
1. `BEAM_OR_RESET` (play-mode-driven)
2. `GET_UP`
3. `SUPPORT_SHAPE`

Transitions:
- Beam/reset modes preempt role logic:
  - `environment/3d-environment/FCPCodebase/agent/Agent.py:228`
  - `environment/3d-environment/FCPCodebase/agent/Agent.py:230`
- Fallen/get-up preempts role logic:
  - `environment/3d-environment/FCPCodebase/agent/Agent.py:232`
- If inactive and not goalie, enter `SUPPORT_SHAPE`:
  - `environment/3d-environment/FCPCodebase/agent/Agent.py:241`
  - `environment/3d-environment/FCPCodebase/agent/Agent.py:245`

Support policy:
- Compute x-position from ball x and own initial x:
  - `new_x = max(0.5,(ball_x+15)/15) * (init_x+15) - 15`
  - `environment/3d-environment/FCPCodebase/agent/Agent.py:246`
- If team appears to have possession (`min_team_dist < min_opp_dist`), push line forward:
  - `new_x = min(new_x + 3.5, 13)`
  - `environment/3d-environment/FCPCodebase/agent/Agent.py:247`
- Move to `(new_x, init_y)` facing ball, yielding priority to active player in obstacle planner:
  - `environment/3d-environment/FCPCodebase/agent/Agent.py:249`

### Goalkeeper FSM (3D)

Definition used here:
- Goalkeeper is primarily `unum=1`.

States:
1. `BEAM_OR_RESET`
2. `GET_UP`
3. `GOALKEEPER_HOLD`
4. `ACTIVE_STRIKER_FALLBACK`

Transitions:
- Beam and get-up transitions are shared with all roles:
  - `environment/3d-environment/FCPCodebase/agent/Agent.py:228`
  - `environment/3d-environment/FCPCodebase/agent/Agent.py:232`
- If goalkeeper is inactive (`active_player_unum != 1`):
  - `-> GOALKEEPER_HOLD`, move to init position and face ball
  - `environment/3d-environment/FCPCodebase/agent/Agent.py:242`
- If goalkeeper becomes active (`active_player_unum == 1`):
  - enters striker controller path (`ACTIVE_STRIKER_FALLBACK`)
  - `environment/3d-environment/FCPCodebase/agent/Agent.py:251`
  - special guard sends it to `RECOVER` on opponent-kick mode group:
    - `environment/3d-environment/FCPCodebase/agent/Agent.py:25`

## Notes on strategy-conditioned role behavior

2D strategy parameters:
- Team name maps to `formation_id`, `press_threshold`, `shoot_range`:
  - `environment/2d-environment/starter-stack/Agent/src/sample_player.cpp:95`

3D strategy parameters:
- Strategy name maps to same semantic triple (`formation_id`, `press_threshold`, `shoot_range`):
  - `environment/3d-environment/FCPCodebase/strategy/strategy_registry.py:4`
- Mapped into runtime control parameters:
  - `press_threshold -> press_margin`:
    - `environment/3d-environment/FCPCodebase/strategy/press_mapping.py:1`
  - `shoot_range -> goal_shot_dist_thresh_m`:
    - `environment/3d-environment/FCPCodebase/strategy/shoot_mapping.py:1`
  - wiring into `Agent(...)`:
    - `environment/3d-environment/FCPCodebase/Run_Player.py:21`
    - `environment/3d-environment/FCPCodebase/Run_Player.py:61`

## Optional: penalty mode caveat (3D)

`Run_Player.py -P` uses `Agent_Penalty.py`, which has a different role FSM:
- goalkeeper dive-left/dive-right/wait states, kicker wait-and-kick policy:
  - `environment/3d-environment/FCPCodebase/agent/Agent_Penalty.py:7`
