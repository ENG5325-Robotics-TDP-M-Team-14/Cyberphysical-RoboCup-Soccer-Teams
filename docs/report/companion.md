# ENG5325 Report Companion

This repository is the public software companion to Team 14's ENG5325 final
report. The report is maintained separately from this repo; this page explains
how the report's technical narrative maps back to source code, scripts, and
public documentation in this repository.

## Scope Boundary

The report focuses on Stage 1 of the assignment:

- simulation setup for RoboCup-derived 2D and 3D environments,
- autonomous team behaviour through strategy and parameter settings,
- strategy-level benchmarking against `BASIC`,
- one-factor-at-a-time parameter experiments for `shoot_range`,
  `press_threshold`, and `formation`,
- role-level interpretation for striker, defender, and goalkeeper behaviour.

Stage 2 hardware transfer is not implemented here. The repository supports
simulation and benchmark reproduction only.

## Code and Documentation Traceability

| Report topic | Repository evidence |
|---|---|
| 2D environment setup | `environment/2d-environment/`, `docs/setup/linux.md` |
| 2D benchmark entrypoints | `environment/2d-environment/starter-stack/run_strategy_benchmark_2d.sh`, `environment/2d-environment/starter-stack/run_parametric_benchmark_2d.sh` |
| 2D strategy/parameter mapping | `environment/2d-environment/starter-stack/Agent/src/sample_player.cpp` |
| 2D role behaviour | `environment/2d-environment/starter-stack/Agent/src/sample_player.cpp`, `bhv_basic_move.cpp`, `bhv_basic_offensive_kick.cpp`, `bhv_goalie_basic_move.cpp` |
| 3D environment setup | `environment/3d-environment/`, `docs/setup/linux.md` |
| 3D benchmark entrypoints | `environment/3d-environment/scripts/run_strategy_benchmark_3d.sh`, `environment/3d-environment/scripts/run_parametric_benchmark_3d.sh` |
| 3D strategy/parameter mapping | `environment/3d-environment/FCPCodebase/strategy/strategy_registry.py`, `press_mapping.py`, `shoot_mapping.py` |
| 3D role behaviour | `environment/3d-environment/FCPCodebase/agent/Agent.py`, `docs/architecture/role-fsm-2d-3d.md` |
| Benchmark semantics | `docs/benchmarks/workflows.md` |

## Strategy and Parameter Alignment

The report's strategy labels are intentionally compact aliases for controller
settings. In both 2D and 3D, the common semantic triple is:

| Strategy | Formation | Press threshold | Shoot range |
|---|---:|---:|---:|
| `BASIC` | `BASELINE` | `3` | `25.0` |
| `NOISE` | `BASELINE` | `3` | `25.0` |
| `DEFLOCK` | `DEF_121` | `2` | `20.0` |
| `HIPRESS` | `DEF_121` | `4` | `20.0` |
| `DIRECT` | `OFF_112` | `2` | `30.0` |
| `AGGRO` | `OFF_112` | `4` | `30.0` |

`NOISE` is a duplicate-baseline alias used as a negative-control concept. It is
not a separate behaviour bundle.

The parameter-level experiments hold `BASIC` fixed and vary exactly one
parameter:

| Parameter | Low / defensive | Baseline | High / offensive |
|---|---:|---:|---:|
| `shoot_range` | `20.0` | `25.0` | `30.0` |
| `press_threshold` | `2` | `3` | `4` |
| `formation` | `DEF_121` | `BASELINE` | `OFF_112` |

## Selected Figures

The figures below are copied from the final report build as lightweight public
documentation aids. They are included to explain the workflow and headline
interpretation, not to replace regenerated benchmark outputs.

| Figure | File |
|---|---|
| Workflow overview | `docs/report/figures/workflow_overview.pdf` |
| Role framework | `docs/report/figures/role_framework_overview.pdf` |
| Strategy goal difference | `docs/report/figures/strategy_goal_difference.pdf` |
| Strategy win rate | `docs/report/figures/strategy_winrate.pdf` |
| Striker shoot-range sensitivity | `docs/report/figures/striker_shoot_range.pdf` |
| Defender press-threshold sensitivity | `docs/report/figures/defender_press_threshold.pdf` |
| Goalkeeper press-threshold sensitivity | `docs/report/figures/goalkeeper_press_threshold.pdf` |

## Reproducibility Boundary

The public repository does not include historical raw run directories, generated
CSV batches, simulator logs, build folders, local spreadsheets, or personal work
folders. Those artifacts are deliberately excluded so a clone behaves like a
clean software distribution.

To reproduce the workflow, set up the simulators using `docs/setup/linux.md`,
then run the commands in `docs/benchmarks/workflows.md`. New outputs will be
written under ignored output directories such as `benchmark_outputs/` and
`strategy_benchmark_logs_*`.

Because RoboCup simulation outcomes can vary with runtime environment, scheduler
timing, and 3D simulator progress, regenerated values should be treated as a new
benchmark batch rather than as byte-for-byte copies of the report's working
files.
