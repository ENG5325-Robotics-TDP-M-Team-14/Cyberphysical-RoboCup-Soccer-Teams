# Linux Setup (2D Stack)

This document captures the Linux/Ubuntu-specific steps to build and run the 2D stack without macOS-hardcoded paths.

## 1) Build rcssserver and rcssmonitor (CMake)

Use the Linux build directories so the generated files are Linux-native.

rcssserver:
```bash
cd environment/2d-environment/rcssserver-19.0.0
mkdir -p build-linux
cd build-linux
cmake ..
make -j"$(nproc)"
```

rcssmonitor:
```bash
cd environment/2d-environment/rcssmonitor-19.0.1
mkdir -p build-linux
cd build-linux
cmake ..
make -j"$(nproc)"
```

Notes:
- If an existing `build` directory was created on macOS, prefer the clean `build-linux` tree above.

## 2) Starter-stack Lib (librcsc)

The `starter-stack/Lib` directory must contain a full autotools setup. If your clone is missing it or has stale config scripts, replace it with a known-good librcsc source and ensure these files exist:
- `configure.ac`, `configure`, `aclocal.m4`, `config/`, `m4/`, `Makefile.in`, `config.h.in`, `setup.sh`

Update `config.guess` and `config.sub` in these locations (copy from `/usr/share/misc`):
- `environment/2d-environment/starter-stack/Lib/config`
- `environment/2d-environment/starter-stack/Agent/config`

Build and install librcsc into the Agent-local prefix:
```bash
cd environment/2d-environment/starter-stack/Lib
autoreconf -i
./configure CXXFLAGS='-std=c++14' --prefix="$(pwd)/../Agent/Lib"
make -j"$(nproc)"
make install
```

## 3) Starter-stack Agent

The Agent build needs the `AX_BOOST_BASE` macro. If missing, copy it from the Lib tree:
- `environment/2d-environment/starter-stack/Lib/m4/ax_boost_base.m4` -> `environment/2d-environment/starter-stack/Agent/m4/`

Then build the agent:
```bash
cd environment/2d-environment/starter-stack/Agent
autoreconf -i
./configure CXXFLAGS='-std=c++14' --with-librcsc="$(pwd)/Lib"
make -j"$(nproc)"
```

## 4) Run scripts (Linux-safe library path)

The launch scripts should use a relative, Linux-safe lib path:
- `environment/2d-environment/starter-stack/Agent/src/start.sh`
- `environment/2d-environment/starter-stack/Agent/src/start-4players.sh`

Both should set:
```bash
LIBPATH="$DIR/../Lib/lib"
```

## 5) Quick run (4v4)

From separate terminals:
```bash
cd environment/2d-environment/rcssserver-19.0.0/build-linux
./rcssserver
```

```bash
cd environment/2d-environment/rcssmonitor-19.0.1/build-linux
./rcssmonitor
```

```bash
cd environment/2d-environment/starter-stack/Agent/src
./start-4players.sh -t BASIC
```

```bash
cd environment/2d-environment/starter-stack/Agent/src
./start-4players.sh -t BASIC
```
