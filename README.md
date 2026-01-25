# Cyberphysical RoboCup Soccer Teams

Team 14's workspace for the Cyberphysical RoboCup Soccer Teams project. This repo vendors the 2D simulator stack and a baseline agent, plus an imitation learning research codebase.

## What is included

- `environment/2d-environment/`: Full 2D stack (rcssserver, rcssmonitor, StarterAgent2D).
- `behaviour_algorithm/rcss2d-opp-imitation-main/`: Python/Poetry pipeline for imitation learning (not wired into the agent yet).
- `environment/3d-environment/`: FCP 3D codebase (`FCPCodebase`) plus SimSpark server.
- `model/`: Placeholder (empty currently).

## Quickstart (2D, 4v4)

From the 2D stack, you can run a 4v4 match where both teams use the current "basic" agent.

Build the agent (once, or after changes):
- `./makeAgent.sh` from `environment/2d-environment/starter-stack`

1. Start the server:
   - `./rcssserver` from `environment/2d-environment/rcssserver-19.0.0/build`
2. Start the monitor:
   - `./rcssmonitor` from `environment/2d-environment/rcssmonitor-19.0.1/build`
3. Start the left team:
   - `./start-4players.sh -t BASIC` from `environment/2d-environment/starter-stack/Agent/src`
4. Start the right team:
   - `./start-4players.sh -t BASIC` from `environment/2d-environment/starter-stack/Agent/src`

Stop a match with Ctrl+C or:
- `killall sample_player`
- `killall sample_coach`
- `killall rcssserver`

## Strategy switching (team name -> config)

Strategy is selected solely by the team name passed via `-t`. Behaviors do not parse names directly; they read a `StrategyConfig` created once in `sample_player.cpp`.

Strategy parameters in `StrategyConfig`:
- `formation_id`: `DEF_121` or `OFF_112` (baseline uses the existing formation path).
- `press_threshold`: integer (low/high press).
- `shoot_range`: distance threshold (conservative/aggressive).

Where these are wired:
- Strategy mapping: `environment/2d-environment/starter-stack/Agent/src/sample_player.cpp`
- Formation hook: `environment/2d-environment/starter-stack/Agent/src/bhv_basic_move.cpp`
- Press threshold hook: `environment/2d-environment/starter-stack/Agent/src/bhv_basic_move.cpp`
- Shooting range hook: `environment/2d-environment/starter-stack/Agent/src/bhv_basic_offensive_kick.cpp`

Canonical strategy team names (<= 12 chars):
- `BASIC` (baseline)
- `NOISE` (baseline, control)
- `DEFLOCK` (DEF_121, low press, conservative shoot)
- `HIPRESS` (DEF_121, high press, conservative shoot)
- `DIRECT` (OFF_112, low press, aggressive shoot)
- `AGGRO` (OFF_112, high press, aggressive shoot)

## 3D simulator stack (current)

- Linux stack using SimSpark/rcssserver3d plus RoboViz.
- Server + monitor launcher: `environment/3d-environment/scripts/run_rcssserver3d_and_RoboViz.sh`.
- RoboViz config: `environment/3d-environment/RoboViz/config.txt` (log recording enabled to the 3D benchmark log directory).

## 3D strategy selection (current)

- Agents are launched with `--strategy` (e.g., `BASIC`, `NOISE`, `DEFLOCK`, `HIPRESS`, `DIRECT`, `AGGRO`).
- Strategy config is defined in `environment/3d-environment/FCPCodebase/strategy/strategy_registry.py` and attached to the player at startup.
- Lever mappings:
  - Formation: `environment/3d-environment/FCPCodebase/world/commons/formations.py` and `environment/3d-environment/FCPCodebase/agent/Agent.py`.
  - Press threshold: `environment/3d-environment/FCPCodebase/strategy/press_mapping.py` and the engage decision in `environment/3d-environment/FCPCodebase/agent/Agent.py`.
  - Shoot range: `environment/3d-environment/FCPCodebase/strategy/shoot_mapping.py` and the goal-directed kick decision in `environment/3d-environment/FCPCodebase/agent/Agent.py`.

## Strategy benchmark (3D, current)

Script: `environment/3d-environment/scripts/run_strategy_benchmark_3d.sh`

Design:
- BASIC vs NOISE/DEFLOCK/HIPRESS/DIRECT/AGGRO
- Sides swapped
- Replicates per side: 5 (configurable via `--repeats`)

Execution notes:
- Each match starts a fresh rcssserver3d + RoboViz instance.
- Match end is detected by parsing RoboViz logs (half-time reached).
- Key options: `--repeats`, `--half-time-timeout-sec`, `MATCH_WALL_TIMEOUT_SEC`, `PROGRESS_INTERVAL_SEC`.

## 3D results & logs

- CSV: `environment/3d-environment/strategy_benchmark_logs_3d/strategy_benchmark_results_3d.csv`
  - Columns: `match_id,pair_id,left_team,right_team,left_goals,right_goals,timestamp,status,error_reason`
- RoboViz logs: `environment/3d-environment/strategy_benchmark_logs_3d/match_logs/`
- Parser: `scripts/utils/parse_roboviz_log.py` (reads `score_left/score_right` from world-state lines).

## Known limitations (3D)

- Physical kick motion is unstable; robots can fall during kicks.
- RoboViz logging can be heavy; log parsing is required for match timing.
- VM environments often emit OpenGL/llvmpipe warnings and may run slower than real time.

## Strategy benchmark (DoE runner)

Script: `environment/2d-environment/starter-stack/run_strategy_benchmark.sh`

Design:
- Pairs: BASIC vs NOISE/DEFLOCK/HIPRESS/DIRECT/AGGRO
- Sides: swap left/right
- Replicates: 5 per side
- Total matches: 50

Output:
- CSV: `environment/2d-environment/starter-stack/strategy_benchmark_results.csv`
  - Columns: `pair_id,left_team,right_team,left_goals,right_goals,timestamp`
- Logs: `environment/2d-environment/starter-stack/strategy_benchmark_log/` (`.rcg`, `.rcl`)

Defaults in the runner:
- Headless server (no monitor required)
- `server::auto_mode=on`
- `server::half_time=150` (~3000 cycles total, ~5 min sim time)
- Hard timeout: 300s wall-clock (set `MATCH_TIMEOUT_SECONDS=...` to override)

Example run (from `environment/2d-environment/starter-stack`):
```bash
./run_strategy_benchmark.sh
```

## Where to change behavior (strategy)

The baseline agent is in StarterAgent2D:
- Main decision loop: `environment/2d-environment/starter-stack/Agent/src/sample_player.cpp`
- Movement/positioning: `environment/2d-environment/starter-stack/Agent/src/bhv_basic_move.cpp`
- Offensive actions: `environment/2d-environment/starter-stack/Agent/src/bhv_basic_offensive_kick.cpp`
- Goalie behavior: `environment/2d-environment/starter-stack/Agent/src/bhv_goalie_basic_move.cpp`
- 4v4 config: `environment/2d-environment/starter-stack/Agent/src/player-4players.conf`
- 4v4 launch script: `environment/2d-environment/starter-stack/Agent/src/start-4players.sh`

## Notes

- The 2D stack is vendored third-party code; keep the original licenses in place.
- The imitation learning project is research-oriented and currently separate from the agent runtime.
- Team names must be <= 12 chars due to RCSS2D protocol limits.
