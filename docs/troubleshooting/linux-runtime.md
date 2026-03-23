# Linux runtime troubleshooting

This page collects the main teammate-facing issues for the current Linux workflow.

## 2D: `sample_player` / `sample_coach` missing or not runnable

Symptom:

- `start-4players.sh`, `run_strategy_benchmark_2d.sh`, or `run_parametric_benchmark_2d.sh` exits immediately and tells you the runtime binary is missing or not runnable.

Cause:

- the maintained `StarterAgent2D-V2` binaries have not been linked into `starter-stack/Agent/src/`,
- or stale binaries for the wrong machine/architecture are still present there.

Fix:

```bash
cd environment/2d-environment/starter-stack
./link_starteragent2d_v2_compat_2d.sh --force
```

If your maintained agent build lives elsewhere:

```bash
cd environment/2d-environment/starter-stack
./link_starteragent2d_v2_compat_2d.sh --bin-dir /path/to/StarterAgent2D-V2/build/bin --lib-dir /path/to/librcsc/lib --force
```

## 2D: both teams join one side

Symptom:

- you launch left and right teams manually, but they all join the same side.

Cause:

- RCSS2D treats identical team names as the same team.

Fix:

- use different team names for the two launches, for example `BASIC` and `AGGRO`.

## 2D: teammate follows old autotools build steps

Symptom:

- someone runs `makeFirstTime.sh`, `makeAll.sh`, or `makeAgent.sh` and hits a warning/exit.

Cause:

- that older starter-stack source build is intentionally marked legacy in this repo.

Fix:

- use the maintained CMake-based `StarterLibRCSC-V2` + `StarterAgent2D-V2` path from [../setup/linux.md](../setup/linux.md),
- only set `LEGACY_STARTER_STACK_BUILD=1` if you already maintain the old environment on purpose.

## 2D: fresh clone build path does not match old librcsc instructions

Symptom:

- autotools metadata is missing,
- old split `librcsc_*` libraries are expected,
- newer librcsc APIs do not match the old source tree.

Fix:

- do not use the legacy `starter-stack/Lib` path as the default build route,
- use the maintained CMake-based pair instead.

## 3D: benchmark scripts complain about `systemd-inhibit`

Symptom:

- 3D benchmark scripts fail in a container, remote shell, or Linux session without a working DBus user session.

Fix:

```bash
cd environment/3d-environment/scripts
BENCH_NO_INHIBIT=1 ./run_strategy_benchmark_3d.sh
```

The same environment variable can be used with the 3D parametric runner.

## 3D: no RoboViz-backed benchmark progress

Symptom:

- 3D matches do not progress as expected or logging is incomplete.

Cause:

- the supported benchmark workflow depends on RoboViz-backed logging.

Fix:

- ensure RoboViz is available under `environment/3d-environment/RoboViz/`,
- use the normal GUI-backed path for benchmark collection.

## Port conflicts

Symptom:

- simulator startup fails because a benchmark port is already in use.

Fix:

- stop older runs,
- or set a different `RCSSSERVER_PORT_BASE` for 2D benchmark scripts.
