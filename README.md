# Cyberphysical RoboCup Soccer Teams

Technical workspace for Team 14's cyberphysical RoboCup work.

This repository contains three parallel tracks:
1. 2D simulator runtime and baseline C++ agent (StarterAgent2D-based).
2. 3D simulator runtime and FCP Python agent (SimSpark + RoboViz + FCPCodebase).
3. Offline imitation-learning research pipeline for RCSS2D logs.

It also contains two benchmark families:
- Strategy benchmarking: compare bundled controller presets against each other.
- Parametric benchmarking: keep a fixed baseline controller and vary one parameter at a time.

## Overview

Use this repo when you need to:
- run vanilla/sandbox 2D or 3D simulation matches,
- run controlled strategy-vs-strategy benchmark suites,
- run one-parameter-isolation benchmark sweeps,
- inspect runtime behavior hooks in 2D C++ and 3D Python agents,
- work on the separate imitation-learning pipeline.

## Repository Map

| Path | Purpose |
|---|---|
| `environment/2d-environment/` | RCSS2D server/monitor + StarterAgent2D runtime stack |
| `environment/2d-environment/starter-stack/Agent/src/` | 2D agent behavior code and launch scripts |
| `environment/2d-environment/starter-stack/run_strategy_benchmark.sh` | 2D strategy benchmark runner (4v4, fixed pair set) |
| `environment/2d-environment/starter-stack/run_parametric_benchmark.sh` | 2D parametric benchmark runner (4v4 + 1v1 modes) |
| `environment/3d-environment/FCPCodebase/` | 3D team runtime code (Python + C++ modules) |
| `environment/3d-environment/scripts/run_strategy_benchmark_3d.sh` | 3D strategy benchmark runner |
| `environment/3d-environment/scripts/run_parametric_benchmark_3d.sh` | 3D parametric benchmark runner |
| `scripts/utils/parse_roboviz_log.py` | RoboViz log parser used by 3D benchmark flow |
| `behaviour_algorithm/rcss2d-opp-imitation-main/` | Offline RCSS2D imitation-learning pipeline |
| `LINUX_SETUP.md` | Linux build/runtime setup guide for 2D stack |
| `docs/ROLE_FSM_2D_3D.md` | Role/FSM behavior specification extracted from code |

## Strategy vs Parametric Benchmarking

- Strategy benchmarking:
  - compares named bundled controller presets (`BASIC`, `DEFLOCK`, `AGGRO`, etc.),
  - used for high-level tactical comparison.

- Parametric benchmarking:
  - fixes baseline controller (`BASIC`),
  - varies one parameter only (`press_threshold`, `shoot_range`, or `formation`),
  - used for factor isolation and sensitivity studies.

## Setup and Prerequisites

## Platform

Linux is the primary target environment for the documented commands.

Read first:
- [LINUX_SETUP.md](LINUX_SETUP.md)

## 2D setup (minimum)

Build server and monitor (Linux-native build dirs):

```bash
cd environment/2d-environment/rcssserver-19.0.0
mkdir -p build-linux
cd build-linux
cmake ..
make -j"$(nproc)"
```

```bash
cd environment/2d-environment/rcssmonitor-19.0.1
mkdir -p build-linux
cd build-linux
cmake ..
make -j"$(nproc)"
```

Build the 2D agent runtime:

```bash
cd environment/2d-environment/starter-stack
./makeAgent.sh
```

If `makeAgent.sh` is insufficient in your environment, follow full librcsc/bootstrap instructions in [LINUX_SETUP.md](LINUX_SETUP.md).

## 3D setup (minimum)

Requirements:
- `rcssserver3d` available on PATH,
- RoboViz available under `environment/3d-environment/RoboViz/`,
- Python venv for FCPCodebase (`environment/3d-environment/FCPCodebase/.venv`).

The 3D benchmark scripts build/check C++ helper modules automatically when needed.

## Common Workflows

## 1) Sandbox / Vanilla Sims

### 2D vanilla sandbox (interactive 4v4)

Terminal 1:

```bash
cd environment/2d-environment/rcssserver-19.0.0/build-linux
./rcssserver
```

Terminal 2:

```bash
cd environment/2d-environment/rcssmonitor-19.0.1/build-linux
./rcssmonitor
```

Terminal 3 (left team):

```bash
cd environment/2d-environment/starter-stack/Agent/src
./start-4players.sh -t BASIC
```

Terminal 4 (right team):

```bash
cd environment/2d-environment/starter-stack/Agent/src
./start-4players.sh -t BASIC
```

### 3D vanilla sandbox

Option A (quick smoke run managed by script):

```bash
cd environment/3d-environment/scripts
./smoke_strategy_demo.sh
```

Option B (manual launcher + explicit team processes):

Terminal 1:

```bash
cd environment/3d-environment/scripts
./run_rcssserver3d_and_RoboViz.sh
```

Terminal 2:

```bash
cd environment/3d-environment/FCPCodebase
source .venv/bin/activate
for u in 1 2 3 4; do python Run_Player.py -t Home -u "$u" --strategy BASIC & done
for u in 1 2 3 4; do python Run_Player.py -t Away -u "$u" --strategy BASIC & done
```

## 2) 4v4 Strategy Benchmarking

### 2D (fixed 4v4 benchmark suite)

```bash
cd environment/2d-environment/starter-stack
./run_strategy_benchmark.sh
```

### 3D (default suite)

```bash
cd environment/3d-environment/scripts
./run_strategy_benchmark_3d.sh
```

If your environment has no working `systemd-inhibit` DBus session (common in containers), run:

```bash
cd environment/3d-environment/scripts
BENCH_NO_INHIBIT=1 ./run_strategy_benchmark_3d.sh
```

## 3) 1v1 Strategy Benchmarking

### 3D (supported by strategy runner)

```bash
cd environment/3d-environment/scripts
./run_strategy_benchmark_3d.sh --pairs BASIC,AGGRO --unums 2 --repeats 5
```

### 2D (partial/manual support)

There is no dedicated 1v1 strategy benchmark script in the 2D flow today. Use manual 1v1 launches (single field player, no coach) for spot checks:

Terminal 1:

```bash
cd environment/2d-environment/rcssserver-19.0.0/build-linux
./rcssserver server::auto_mode=on server::half_time=150
```

Terminal 2:

```bash
cd environment/2d-environment/starter-stack/Agent/src
./start-4players.sh -t BASIC -n 4 -u 2 -C
```

Terminal 3:

```bash
cd environment/2d-environment/starter-stack/Agent/src
./start-4players.sh -t AGGRO -n 4 -u 2 -C
```

This is a manual run path, not the same automated CSV pipeline as `run_strategy_benchmark.sh`.

## 4) 4v4 Parametric Benchmarking

### 2D

```bash
cd environment/2d-environment/starter-stack
./run_parametric_benchmark.sh --parameter press_threshold --mode 4v4 --repeats 5
```

```bash
cd environment/2d-environment/starter-stack
./run_parametric_benchmark.sh --parameter shoot_range --mode 4v4 --repeats 5
```

```bash
cd environment/2d-environment/starter-stack
./run_parametric_benchmark.sh --parameter formation --mode 4v4 --repeats 5 --levels baseline,def,off
```

### 3D

```bash
cd environment/3d-environment/scripts
./run_parametric_benchmark_3d.sh --parameter press_threshold --mode 4v4 --repeats 5
```

```bash
cd environment/3d-environment/scripts
./run_parametric_benchmark_3d.sh --parameter shoot_range --mode 4v4 --repeats 5
```

```bash
cd environment/3d-environment/scripts
./run_parametric_benchmark_3d.sh --parameter formation --mode 4v4 --repeats 5 --levels baseline,def,off
```

## 5) 1v1 Parametric Benchmarking

### 2D

```bash
cd environment/2d-environment/starter-stack
./run_parametric_benchmark.sh --parameter press_threshold --mode 1v1 --repeats 5
```

### 3D

```bash
cd environment/3d-environment/scripts
./run_parametric_benchmark_3d.sh --parameter press_threshold --mode 1v1 --repeats 5
```

For both 2D and 3D parametric flows, `--mode 1v1` currently runs one field player (`unum 2`) per side; formation effects are naturally limited in 1v1.

## Benchmark Outputs

## Strategy benchmark outputs

### 2D strategy output
- CSV: `environment/2d-environment/starter-stack/strategy_benchmark_results.csv`
- Match logs: `environment/2d-environment/starter-stack/strategy_benchmark_log/`

### 3D strategy output
- Default root: `environment/3d-environment/strategy_benchmark_logs_3d/`
- CSV: `strategy_benchmark_results_3d.csv`
- Match logs: `match_logs/`

You can override 3D output locations with `--out-csv` and `--log-dir`.

## Parametric benchmark outputs (standardized)

### 2D parametric output root
- `environment/2d-environment/starter-stack/benchmark_outputs/2d/parametric/...`

### 3D parametric output root
- `environment/3d-environment/benchmark_outputs/3d/parametric/...`

Structure (both sims):

```text
.../benchmark_outputs/<2d|3d>/parametric/
  mode_<1v1|4v4>/
    baseline_BASIC/
      parameter_<press_threshold|shoot_range|formation>/
        run_<NNNN>/
          results.csv
          levels.csv
          run_metadata.json
          metrics_catalog.csv
          behavioural_metrics_scaffold.csv
```

Additional 3D per-level artifacts:

```text
run_<NNNN>/levels/level_<name>/
  strategy_results.csv
  strategy_stdout.log
  strategy_logs/
```

Notes:
- `run_<NNNN>` is deterministic and incrementing per semantic output folder.
- `--out-csv` in parametric scripts mirrors final `results.csv` to your requested path; canonical artifacts remain inside the run folder.

## Behavioral Metrics Status

Currently available from benchmark outputs:
- goals (`left_goals`, `right_goals`),
- run/match status (`status`, `error_reason`),
- timestamps,
- 2D wall-clock duration (`wall_time_sec`).

Scaffolded only (not computed yet):
- shot distance/location,
- press initiation distance,
- time-to-shot,
- time-to-engagement,
- spatial occupancy,
- trajectory/path properties.

These are represented as placeholders and availability flags in:
- `metrics_catalog.csv`
- `behavioural_metrics_scaffold.csv`

## Offline Imitation-Learning Pipeline (Not Runtime-Integrated)

Subproject:
- `behaviour_algorithm/rcss2d-opp-imitation-main/`

Typical usage:

```bash
cd behaviour_algorithm/rcss2d-opp-imitation-main
poetry install
poetry run python cli.py
```

This pipeline is research-oriented and not currently wired into the live 2D or 3D runtime agents.

## Key Code Entry Points

2D:
- `environment/2d-environment/starter-stack/Agent/src/sample_player.cpp`
- `environment/2d-environment/starter-stack/Agent/src/bhv_basic_move.cpp`
- `environment/2d-environment/starter-stack/Agent/src/bhv_basic_offensive_kick.cpp`
- `environment/2d-environment/starter-stack/Agent/src/start-4players.sh`

3D:
- `environment/3d-environment/FCPCodebase/Run_Player.py`
- `environment/3d-environment/FCPCodebase/agent/Agent.py`
- `environment/3d-environment/FCPCodebase/strategy/strategy_registry.py`
- `environment/3d-environment/FCPCodebase/strategy/press_mapping.py`
- `environment/3d-environment/FCPCodebase/strategy/shoot_mapping.py`
- `scripts/utils/parse_roboviz_log.py`

Behavior documentation:
- [docs/ROLE_FSM_2D_3D.md](docs/ROLE_FSM_2D_3D.md)

## Limitations and Caveats

- 2D strategy benchmark script is a fixed 4v4 suite; it does not provide built-in 1v1 mode.
- 2D 1v1 strategy comparisons are currently manual.
- 1v1 mode in parametric benchmarks uses a single field player (`unum 2`), so formation effects are limited.
- 3D benchmark flow depends on RoboViz logs for match progression detection.
- In headless/container environments, `BENCH_NO_INHIBIT=1` may be required for 3D benchmark scripts.
- Team names in RCSS2D should remain within protocol limits (<= 12 chars).
- Parametric behavioral metrics beyond score/status are scaffolded placeholders until event-level telemetry is added.

## Licensing and Third-Party Code

Large parts of the 2D and 3D simulation stacks are vendored third-party code. Preserve original license files and attribution when modifying or redistributing components.
