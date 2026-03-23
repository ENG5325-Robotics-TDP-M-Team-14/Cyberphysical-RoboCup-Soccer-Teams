# Cyberphysical RoboCup Soccer Teams

Linux-first workspace for Team 14's RoboCup simulation work.

This repository combines three tracks in one place:

1. 2D simulator runtime and benchmarking.
2. 3D simulator runtime and benchmarking.
3. An offline RCSS2D imitation-learning pipeline.

The root `README.md` is the canonical project entrypoint. Start here, then use the linked `docs/` pages for deeper setup, benchmark, troubleshooting, and architecture details.

## What This Repository Is

This is a workspace, not a single application. It contains:

- a 2D runtime stack built around the starter-stack launch and benchmark scripts,
- a 3D runtime stack built around the FC Portugal Python codebase plus SimSpark/RoboViz,
- benchmark tooling for strategy-level and parameter-level experiments,
- a separate offline learning pipeline for RCSS2D logs.

## Repository Architecture

| Path | Purpose |
|---|---|
| `environment/2d-environment/` | RCSS2D server, monitor, and 2D runtime/benchmark assets |
| `environment/2d-environment/starter-stack/` | 2D launch scripts, benchmark runners, compatibility bridge, and analyzed behavior code |
| `environment/3d-environment/` | SimSpark, RoboViz, FCPCodebase, and 3D benchmark scripts |
| `behaviour_algorithm/rcss2d-opp-imitation-main/` | Offline imitation-learning subproject |
| `docs/setup/` | Linux-only setup and environment notes |
| `docs/benchmarks/` | Benchmark workflow and methodology notes |
| `docs/architecture/` | Role/FSM and architecture-level documentation |
| `docs/troubleshooting/` | Common teammate issues and fixes |
| `docs/development/` | Code entry points and extension notes |

## Supported Platform

Linux is the only onboarding path documented and supported in this repository.

If you are setting up the project for the first time, read [docs/setup/linux.md](docs/setup/linux.md) after this README.

## Quick Start

1. Build the 2D simulator binaries in `environment/2d-environment/rcssserver-19.0.0/build-linux` and `environment/2d-environment/rcssmonitor-19.0.1/build-linux`.
2. Build `StarterLibRCSC-V2` and `StarterAgent2D-V2`, then refresh the 2D compatibility bridge with `environment/2d-environment/starter-stack/link_starteragent2d_v2_compat_2d.sh --force`.
3. Ensure 3D prerequisites are available: `rcssserver3d` on `PATH`, RoboViz under `environment/3d-environment/RoboViz/`, and the FCPCodebase virtual environment.
4. Use the benchmark runners for repeatable experiments or the start scripts for manual sandbox runs.

Detailed Linux setup lives in [docs/setup/linux.md](docs/setup/linux.md).

## Core Dependencies And Setup Orientation

### 2D

- Vendored `rcssserver` and `rcssmonitor` are built from this repo with CMake.
- The supported 2D agent build path is the maintained CMake-based `StarterLibRCSC-V2` + `StarterAgent2D-V2` pair.
- The starter-stack launchers still expect `sample_player`, `sample_coach`, and `sample_trainer` under `environment/2d-environment/starter-stack/Agent/src/`, so the compatibility helper script links the maintained binaries into that legacy location.

### 3D

- `rcssserver3d` must be installed and runnable on the machine.
- RoboViz is used as part of the supported benchmark flow.
- The Python runtime lives under `environment/3d-environment/FCPCodebase/.venv`.

### Offline Pipeline

- The imitation-learning project is separate from the live 2D and 3D runtime agents.
- It uses Poetry and has its own subproject README for component-specific details.

## 2D Setup Summary

Build the vendored simulator tools:

```bash
cd environment/2d-environment/rcssserver-19.0.0
mkdir -p build-linux
cd build-linux
cmake ..
cmake --build . -j"$(nproc)"
```

```bash
cd environment/2d-environment/rcssmonitor-19.0.1
mkdir -p build-linux
cd build-linux
cmake ..
cmake --build . -j"$(nproc)"
```

Build the maintained lib and agent pair:

```bash
cd environment/2d-environment
git clone https://github.com/RCSS-IR/StarterLibRCSC-V2.git
git clone https://github.com/RCSS-IR/StarterAgent2D-V2.git
```

```bash
cd environment/2d-environment/StarterLibRCSC-V2
mkdir -p build
cd build
cmake ..
cmake --build . -j"$(nproc)"
cmake --install .
```

```bash
cd environment/2d-environment/StarterAgent2D-V2
mkdir -p build
cd build
cmake -DLIBRCSC_INSTALL_DIR="$HOME/local/starter" ..
cmake --build . -j"$(nproc)"
```

Bridge the maintained binaries into the starter-stack runtime layout:

```bash
cd environment/2d-environment/starter-stack
./link_starteragent2d_v2_compat_2d.sh --force
```

Detailed setup, custom install-prefix notes, and the legacy build status are documented in [docs/setup/linux.md](docs/setup/linux.md).

## 3D Setup Summary

The repo assumes the following are already available on the Linux machine:

- `rcssserver3d` on `PATH`,
- RoboViz under `environment/3d-environment/RoboViz/`,
- a working FCPCodebase virtual environment at `environment/3d-environment/FCPCodebase/.venv`.

Typical activation:

```bash
cd environment/3d-environment/FCPCodebase
source .venv/bin/activate
```

The 3D benchmark scripts build or verify their helper modules as needed. See [docs/setup/linux.md](docs/setup/linux.md) and [docs/troubleshooting/linux-runtime.md](docs/troubleshooting/linux-runtime.md) for environment caveats.

## Main Workflows

### Vanilla / Sandbox Runs

#### 2D vanilla 4v4

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

Terminal 3:

```bash
cd environment/2d-environment/starter-stack/Agent/src
./start-4players.sh -t BASIC
```

Terminal 4:

```bash
cd environment/2d-environment/starter-stack/Agent/src
./start-4players.sh -t AGGRO
```

Important: in RCSS2D, left and right teams must use different team names.

#### 3D vanilla run

Quick managed smoke:

```bash
cd environment/3d-environment/scripts
./smoke_strategy_demo.sh
```

Manual launcher:

```bash
cd environment/3d-environment/scripts
./run_rcssserver3d_and_RoboViz.sh
```

```bash
cd environment/3d-environment/FCPCodebase
source .venv/bin/activate
for u in 1 2 3 4; do python Run_Player.py -t Home -u "$u" --strategy BASIC & done
for u in 1 2 3 4; do python Run_Player.py -t Away -u "$u" --strategy BASIC & done
```

### 2D Benchmarking

Project note: for 2D small-sided isolated benchmarking, the recommended duel-style mode is `2v2` because each side includes one field player and one goalkeeper. `1v1` still exists as an optional no-goalie microbenchmark.

#### 2D 4v4 strategy benchmarking

```bash
cd environment/2d-environment/starter-stack
./link_starteragent2d_v2_compat_2d.sh --force
./run_strategy_benchmark_2d.sh
```

#### 2D 1v1 parametric benchmarking

```bash
cd environment/2d-environment/starter-stack
./run_parametric_benchmark_2d.sh --parameter press_threshold --mode 1v1 --repeats 5
```

#### 2D 2v2 parametric benchmarking

```bash
cd environment/2d-environment/starter-stack
./run_parametric_benchmark_2d.sh --parameter press_threshold --mode 2v2 --repeats 5
```

#### 2D 4v4 parametric benchmarking

```bash
cd environment/2d-environment/starter-stack
./run_parametric_benchmark_2d.sh --parameter formation --mode 4v4 --levels baseline,def,off --repeats 5
```

### 3D Benchmarking

#### 3D 4v4 strategy benchmarking

```bash
cd environment/3d-environment/scripts
./run_strategy_benchmark_3d.sh
```

If DBus-backed `systemd-inhibit` is unavailable:

```bash
cd environment/3d-environment/scripts
BENCH_NO_INHIBIT=1 ./run_strategy_benchmark_3d.sh
```

#### 3D 1v1 parametric benchmarking

```bash
cd environment/3d-environment/scripts
./run_parametric_benchmark_3d.sh --parameter press_threshold --mode 1v1 --repeats 5
```

#### 3D 2v2 parametric benchmarking

```bash
cd environment/3d-environment/scripts
./run_parametric_benchmark_3d.sh --parameter press_threshold --mode 2v2 --repeats 5
```

#### 3D 4v4 parametric benchmarking

```bash
cd environment/3d-environment/scripts
./run_parametric_benchmark_3d.sh --parameter formation --mode 4v4 --levels baseline,def,off --repeats 5
```

### Offline Imitation-Learning Pipeline

```bash
cd behaviour_algorithm/rcss2d-opp-imitation-main
poetry install
poetry run python cli.py
```

This pipeline is research-oriented and separate from the live benchmark runners.

## Output Locations

- 2D strategy: `environment/2d-environment/starter-stack/strategy_benchmark_results_2d.csv` and `environment/2d-environment/starter-stack/strategy_benchmark_logs_2d/`
- 2D parametric: `environment/2d-environment/starter-stack/benchmark_outputs/2d/parametric/`
- 3D strategy: `environment/3d-environment/strategy_benchmark_logs_3d/` plus optional `--out-csv`
- 3D parametric: `environment/3d-environment/benchmark_outputs/3d/parametric/`

Detailed benchmark semantics and output structure are documented in [docs/benchmarks/workflows.md](docs/benchmarks/workflows.md).

## Documentation Map

- [docs/setup/linux.md](docs/setup/linux.md): Linux-only setup, build details, and the supported 2D compatibility bridge.
- [docs/benchmarks/workflows.md](docs/benchmarks/workflows.md): benchmark modes, recommended usage, and output structure.
- [docs/architecture/role-fsm-2d-3d.md](docs/architecture/role-fsm-2d-3d.md): code-faithful 2D and 3D role/FSM logic.
- [docs/troubleshooting/linux-runtime.md](docs/troubleshooting/linux-runtime.md): teammate-facing setup and runtime failure modes.
- [docs/development/extension-notes.md](docs/development/extension-notes.md): code entry points and where to extend behaviors or benchmark mappings.

## Current Limitations / Caveats

- Linux is the only documented onboarding platform.
- The supported 2D build path is the maintained CMake-based lib/agent pair plus the compatibility bridge. The old starter-stack autotools build is legacy.
- 2D strategy benchmarking is a fixed 4v4 suite. Reduced-player 2D strategy checks are manual only.
- 2D `1v1` parametric mode is a no-goalie microbenchmark. For project-style isolated evaluation, use `2v2`.
- 3D benchmarking is RoboViz-backed rather than fully headless.
- In some Linux environments, 3D scripts need `BENCH_NO_INHIBIT=1`.
- The offline imitation-learning pipeline is not wired into the live runtime agents.
