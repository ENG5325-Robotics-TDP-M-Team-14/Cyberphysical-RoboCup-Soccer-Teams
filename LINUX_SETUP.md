# Linux Setup (Supported 2D Workflow)

This document records the supported Ubuntu/Linux path for the repo's 2D stack.

Use this workflow on a fresh clone:

1. build the vendored `rcssserver` and `rcssmonitor` with CMake,
2. build the maintained `StarterLibRCSC-V2` and `StarterAgent2D-V2` with CMake,
3. link the resulting `sample_*` binaries into `environment/2d-environment/starter-stack/Agent/src/`,
4. run the existing starter-stack launch and benchmark scripts against those linked binaries.

The older `starter-stack/Lib` + `starter-stack/Agent` autotools path is legacy and should not be the default teammate setup.

## 1) Ubuntu packages

```bash
sudo apt update
sudo apt install -y build-essential cmake libboost-all-dev libtool qtbase5-dev libaudio-dev libgtk-3-dev libxt-dev bison flex
```

## 2) Build rcssserver and rcssmonitor

Use Linux-native build directories so generated files are not reused from a different platform.

rcssserver:

```bash
cd environment/2d-environment/rcssserver-19.0.0
mkdir -p build-linux
cd build-linux
cmake ..
cmake --build . -j"$(nproc)"
```

rcssmonitor:

```bash
cd environment/2d-environment/rcssmonitor-19.0.1
mkdir -p build-linux
cd build-linux
cmake ..
cmake --build . -j"$(nproc)"
```

## 3) Clone and build the maintained 2D lib + agent

Clone both maintained repos into `environment/2d-environment/`:

```bash
cd environment/2d-environment
git clone https://github.com/RCSS-IR/StarterLibRCSC-V2.git
git clone https://github.com/RCSS-IR/StarterAgent2D-V2.git
```

Build and install `StarterLibRCSC-V2`:

```bash
cd environment/2d-environment/StarterLibRCSC-V2
mkdir -p build
cd build
cmake ..
cmake --build . -j"$(nproc)"
cmake --install .
```

By default this installs librcsc into `~/local/starter/`.

If you want a different install prefix, use an explicit install-time override and pass the same location into `StarterAgent2D-V2` and the compatibility helper:

```bash
cmake --install . --prefix /path/to/starter-librcsc
```

Build `StarterAgent2D-V2` against that install:

```bash
cd environment/2d-environment/StarterAgent2D-V2
mkdir -p build
cd build
cmake -DLIBRCSC_INSTALL_DIR="$HOME/local/starter" ..
cmake --build . -j"$(nproc)"
```

The resulting binaries should be under:

- `environment/2d-environment/StarterAgent2D-V2/build/bin/sample_player`
- `environment/2d-environment/StarterAgent2D-V2/build/bin/sample_coach`
- `environment/2d-environment/StarterAgent2D-V2/build/bin/sample_trainer`

## 4) Create the starter-stack compatibility bridge

The repo's launch and benchmark scripts still look for `sample_player`, `sample_coach`, and `sample_trainer` in `environment/2d-environment/starter-stack/Agent/src/`.

Run the helper once after each rebuild of `StarterAgent2D-V2`:

```bash
cd environment/2d-environment/starter-stack
./link_starteragent2d_v2_compat_2d.sh --force
```

If your maintained agent or lib build lives somewhere else, pass explicit paths:

```bash
cd environment/2d-environment/starter-stack
./link_starteragent2d_v2_compat_2d.sh \
  --bin-dir ../StarterAgent2D-V2/build/bin \
  --lib-dir "$HOME/local/starter/lib" \
  --force
```

This helper:

- creates or refreshes `sample_player`, `sample_coach`, and `sample_trainer` symlinks in `Agent/src/`,
- writes `Agent/src/.starteragent2d_v2_compat.env` so `start-4players.sh` exports the correct librcsc runtime path,
- keeps the existing starter-stack launch scripts and benchmark scripts usable without porting the old source tree.

## 5) Quick 2D run (4v4)

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

Important: RCSS2D team names must differ across the two launches. If both launches use the same team name, the server treats them as the same team and players join one side.

## 6) 2D benchmark entrypoints

Once the compatibility bridge is in place, the supported benchmark entrypoints are:

```bash
cd environment/2d-environment/starter-stack
./run_strategy_benchmark_2d.sh
```

```bash
cd environment/2d-environment/starter-stack
./run_parametric_benchmark_2d.sh --parameter press_threshold --mode 2v2 --repeats 5
```

## 7) Legacy path: retained, not supported as the primary build

Treat the following as legacy or unsupported unless you already maintain a matching older librcsc/autotools environment:

- `environment/2d-environment/starter-stack/Lib`
- `environment/2d-environment/starter-stack/Agent`
- `environment/2d-environment/starter-stack/makeFirstTime.sh`
- `environment/2d-environment/starter-stack/makeAll.sh`
- `environment/2d-environment/starter-stack/makeAgent.sh`

Known reasons:

- the `starter-stack/Lib` autotools path is not the reliable fresh-clone build route teammates are succeeding with,
- the legacy source expects the older split-librcsc layout (`lrcsc_geom`, `lrcsc_rcg`, and related pieces),
- modern librcsc builds expose a different single-library/API surface and produce compilation mismatches against the old source tree.
