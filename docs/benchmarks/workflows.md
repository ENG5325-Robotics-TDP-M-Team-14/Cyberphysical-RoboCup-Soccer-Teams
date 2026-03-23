# Benchmark Workflows

This document expands the benchmark commands from the root [README.md](../../README.md) and explains how to interpret the supported modes.

## Benchmark Families

The repository supports two benchmark families:

- strategy benchmarking: compare named bundled controller presets against each other,
- parametric benchmarking: hold a fixed baseline and vary exactly one parameter at a time.

Supported tunable parameters are:

- `press_threshold`
- `shoot_range`
- `formation`

## 2D Benchmarking

2D benchmark entrypoints live under `environment/2d-environment/starter-stack/`.

Before any 2D benchmark run:

```bash
cd environment/2d-environment/starter-stack
./link_starteragent2d_v2_compat_2d.sh --force
```

### 2D Strategy Benchmarking

The supported automated strategy runner is the 4v4 suite:

```bash
cd environment/2d-environment/starter-stack
./run_strategy_benchmark_2d.sh
```

There is no dedicated automated reduced-player 2D strategy runner. Reduced-player strategy checks are manual only.

### 2D Parametric Modes

`1v1`

- optional no-goalie microbenchmark,
- useful for pure single-player behavior isolation,
- not the preferred project duel benchmark.

`2v2`

- one field player plus one goalkeeper per side,
- the recommended project small-sided / duel-style benchmark,
- the closest reduced-player analogue to 4v4 because keepers remain present.

`4v4`

- three field players plus one goalkeeper per side,
- the team-level benchmark mode,
- the only mode where `formation` is strongly meaningful.

### 2D Parametric Commands

1v1:

```bash
cd environment/2d-environment/starter-stack
./run_parametric_benchmark_2d.sh --parameter press_threshold --mode 1v1 --repeats 5
```

2v2:

```bash
cd environment/2d-environment/starter-stack
./run_parametric_benchmark_2d.sh --parameter press_threshold --mode 2v2 --repeats 5
```

4v4:

```bash
cd environment/2d-environment/starter-stack
./run_parametric_benchmark_2d.sh --parameter formation --mode 4v4 --levels baseline,def,off --repeats 5
```

## 3D Benchmarking

3D benchmark entrypoints live under `environment/3d-environment/scripts/`.

3D benchmarking is supported with RoboViz in the loop. It is not documented here as a fully headless workflow.

### 3D Strategy Benchmarking

Default 4v4 strategy suite:

```bash
cd environment/3d-environment/scripts
./run_strategy_benchmark_3d.sh
```

If `systemd-inhibit` is unavailable in the current Linux session:

```bash
cd environment/3d-environment/scripts
BENCH_NO_INHIBIT=1 ./run_strategy_benchmark_3d.sh
```

### 3D Parametric Modes

`1v1`

- no-goalie microbenchmark,
- useful for tightly isolated active-player behavior.

`2v2`

- one field player plus one goalkeeper per side,
- the small-sided benchmark.

`4v4`

- the fuller team benchmark,
- the main mode for formation comparisons.

### 3D Parametric Commands

1v1:

```bash
cd environment/3d-environment/scripts
./run_parametric_benchmark_3d.sh --parameter press_threshold --mode 1v1 --repeats 5
```

2v2:

```bash
cd environment/3d-environment/scripts
./run_parametric_benchmark_3d.sh --parameter press_threshold --mode 2v2 --repeats 5
```

4v4:

```bash
cd environment/3d-environment/scripts
./run_parametric_benchmark_3d.sh --parameter formation --mode 4v4 --levels baseline,def,off --repeats 5
```

## Output Structure

2D strategy:

- `environment/2d-environment/starter-stack/strategy_benchmark_results_2d.csv`
- `environment/2d-environment/starter-stack/strategy_benchmark_logs_2d/`

2D parametric:

- `environment/2d-environment/starter-stack/benchmark_outputs/2d/parametric/`

3D strategy:

- `environment/3d-environment/strategy_benchmark_logs_3d/`
- optional mirror CSV via `--out-csv`

3D parametric:

- `environment/3d-environment/benchmark_outputs/3d/parametric/`

Shared parametric structure:

```text
.../benchmark_outputs/<2d|3d>/parametric/
  mode_<1v1|2v2|4v4>/
    baseline_BASIC/
      parameter_<press_threshold|shoot_range|formation>/
        run_<NNNN>/
          results.csv
          levels.csv
          run_metadata.json
```

3D additionally writes per-level strategy logs below `levels/level_<name>/`.

## Interpretation Notes

- `formation` is mainly a 4v4 experiment.
- A benchmark run can still contain `timeout` rows; that is benchmark data, not necessarily a wrapper failure.
- In 2D, if you are choosing one reduced-player mode for project evaluation, use `2v2`.
