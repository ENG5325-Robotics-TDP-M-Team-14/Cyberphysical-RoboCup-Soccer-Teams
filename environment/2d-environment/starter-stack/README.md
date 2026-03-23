# Starter-Stack (Legacy Compatibility Layer)

This directory remains in the repo for two reasons:

- it contains the 2D behavior code used for analysis and benchmarking,
- its launch and benchmark scripts are still the project entrypoints for 2D runtime experiments.

It is not the supported place to compile the 2D agent from source on a fresh Ubuntu clone.

## Supported teammate workflow

Use the maintained CMake-based pair instead:

1. build `environment/2d-environment/rcssserver-19.0.0` and `environment/2d-environment/rcssmonitor-19.0.1`,
2. build `StarterLibRCSC-V2` and `StarterAgent2D-V2`,
3. run `./link_starteragent2d_v2_compat_2d.sh` to link `sample_player`, `sample_coach`, and `sample_trainer` into `Agent/src/`.

See:

- [../../../LINUX_SETUP.md](../../../LINUX_SETUP.md)
- [../../../README.md](../../../README.md)

## Legacy path

The older `starter-stack/Lib` and `starter-stack/Agent` autotools build path is kept only as a historical snapshot.

Treat these as legacy or unsupported unless you already maintain a matching older librcsc environment:

- `makeFirstTime.sh`
- `makeAll.sh`
- `makeAgent.sh`
- direct `starter-stack/Lib` + `starter-stack/Agent` source builds

The main known issues are missing autotools metadata in some clones, the older split-librcsc layout, and API mismatches against modern librcsc builds.
