# Linux Setup

This is the detailed Linux-only setup companion to the root [README.md](../../README.md).

Use the root README first for orientation. Use this page when you need the deeper build and environment details.

## Scope

The supported Linux workflow is:

1. build the vendored 2D simulator binaries from this repo,
2. build the maintained 2D lib and agent pair with CMake,
3. bridge those built binaries into the legacy starter-stack runtime layout,
4. ensure 3D runtime prerequisites are present on the machine.

## System Packages

2D build dependencies:

```bash
sudo apt update
sudo apt install -y build-essential cmake libboost-all-dev libtool qtbase5-dev libaudio-dev libgtk-3-dev libxt-dev bison flex
```

Additional 3D runtime/build dependencies vary by machine, but at minimum you need:

- `rcssserver3d` installed and runnable,
- Java/OpenGL support sufficient for RoboViz,
- Python tooling for the FCPCodebase virtual environment.

## Detailed 2D Setup

### 1) Build vendored rcssserver and rcssmonitor

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

### 2) Clone and build the maintained 2D lib + agent

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

By default, upstream installs to `~/local/starter/`.

If you want a different install prefix, use:

```bash
cmake --install . --prefix /path/to/starter-librcsc
```

Then build `StarterAgent2D-V2` against the same lib location:

```bash
cd environment/2d-environment/StarterAgent2D-V2
mkdir -p build
cd build
cmake -DLIBRCSC_INSTALL_DIR="$HOME/local/starter" ..
cmake --build . -j"$(nproc)"
```

Expected binaries:

- `environment/2d-environment/StarterAgent2D-V2/build/bin/sample_player`
- `environment/2d-environment/StarterAgent2D-V2/build/bin/sample_coach`
- `environment/2d-environment/StarterAgent2D-V2/build/bin/sample_trainer`

### 3) Refresh the starter-stack compatibility bridge

The starter-stack runtime and benchmark scripts still execute `sample_player`, `sample_coach`, and `sample_trainer` from `environment/2d-environment/starter-stack/Agent/src/`.

Refresh those links after each rebuild:

```bash
cd environment/2d-environment/starter-stack
./link_starteragent2d_v2_compat_2d.sh --force
```

Custom locations are supported:

```bash
cd environment/2d-environment/starter-stack
./link_starteragent2d_v2_compat_2d.sh \
  --bin-dir ../StarterAgent2D-V2/build/bin \
  --lib-dir "$HOME/local/starter/lib" \
  --force
```

The helper also writes `Agent/src/.starteragent2d_v2_compat.env` so the launcher scripts export the correct librcsc runtime path.

### 4) Verify a manual 2D run

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

Important: RCSS2D left and right teams must use different team names. Reusing the same name causes both launches to join one side.

## Detailed 3D Orientation

The repo does not vendor a full Linux installer for the 3D stack. The supported assumptions are:

- `rcssserver3d` is already installed and on `PATH`,
- RoboViz is available under `environment/3d-environment/RoboViz/`,
- the FCPCodebase virtual environment exists under `environment/3d-environment/FCPCodebase/.venv`.

Typical activation:

```bash
cd environment/3d-environment/FCPCodebase
source .venv/bin/activate
```

If you are validating the benchmark path, the simplest smoke is:

```bash
cd environment/3d-environment/scripts
BENCH_NO_INHIBIT=1 ./run_strategy_benchmark_3d.sh --pairs BASIC,AGGRO --repeats 1
```

See [../troubleshooting/linux-runtime.md](../troubleshooting/linux-runtime.md) for Linux-specific 3D caveats.

## Unsupported 2D Legacy Build Path

The following are retained only as historical compatibility artifacts and are not the supported teammate path:

- `environment/2d-environment/starter-stack/Lib`
- `environment/2d-environment/starter-stack/Agent`
- `environment/2d-environment/starter-stack/makeFirstTime.sh`
- `environment/2d-environment/starter-stack/makeAll.sh`
- `environment/2d-environment/starter-stack/makeAgent.sh`

Known issues with that older path:

- fresh clones can lack or mismatch the expected autotools metadata,
- the old source expects the older split librcsc layout,
- modern librcsc builds expose different APIs and link structure.
