# Cyberphysical RoboCup Soccer Teams

Team 14's workspace for the Cyberphysical RoboCup Soccer Teams project. This repo vendors the 2D simulator stack and a baseline agent, plus an imitation learning research codebase.

## What is included

- `environment/2d-environment/`: Full 2D stack (rcssserver, rcssmonitor, StarterAgent2D).
- `behaviour_algorithm/rcss2d-opp-imitation-main/`: Python/Poetry pipeline for imitation learning (not wired into the agent yet).
- `environment/3d-environment/`: Placeholder (empty currently).
- `model/`: Placeholder (empty currently).

## Quickstart (2D, 4v4)

From the 2D stack, you can run a 4v4 match where both teams use the current "basic" agent.

1. Start the server:
   - `./rcssserver` from `environment/2d-environment/rcssserver-19.0.0/build`
2. Start the monitor:
   - `./rcssmonitor` from `environment/2d-environment/rcssmonitor-19.0.1/build`
3. Start the left team:
   - `./start-4players.sh -t LEFT` from `environment/2d-environment/starter-stack/Agent/src`
4. Start the right team:
   - `./start-4players.sh -t RIGHT` from `environment/2d-environment/starter-stack/Agent/src`

Stop a match with Ctrl+C or:
- `killall sample_player`
- `killall sample_coach`
- `killall rcssserver`

## Where to change behavior (strategy)

The baseline agent is in StarterAgent2D:
- Main decision loop: `environment/2d-environment/starter-stack/Agent/src/sample_player.cpp`
- Movement/positioning: `environment/2d-environment/starter-stack/Agent/src/bhv_basic_move.cpp`
- Offensive actions: `environment/2d-environment/starter-stack/Agent/src/bhv_basic_offensive_kick.cpp`
- Goalie behavior: `environment/2d-environment/starter-stack/Agent/src/bhv_goalie_basic_move.cpp`
- 4v4 config: `environment/2d-environment/starter-stack/Agent/src/player-4players.conf`
- 4v4 launch script: `environment/2d-environment/starter-stack/Agent/src/start-4players.sh`

Canonical strategy team names (<= 12 chars):
- `BASIC` (baseline)
- `NOISE` (baseline, control)
- `DEFLOCK` (DEF_121, low press, conservative shoot)
- `HIPRESS` (DEF_121, high press, conservative shoot)
- `DIRECT` (OFF_112, low press, aggressive shoot)
- `AGGRO` (OFF_112, high press, aggressive shoot)

## Notes

- The 2D stack is vendored third-party code; keep the original licenses in place.
- The imitation learning project is research-oriented and currently separate from the agent runtime.
- Team names must be <= 12 chars due to RCSS2D protocol limits.
