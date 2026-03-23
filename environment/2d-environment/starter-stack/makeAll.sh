#!/bin/bash
set -e

if [[ "${LEGACY_STARTER_STACK_BUILD:-0}" != "1" ]]; then
  echo "Warning: makeAll.sh rebuilds the legacy starter-stack Lib/Agent source tree." >&2
  echo "The supported teammate workflow is the CMake-based StarterLibRCSC-V2 + StarterAgent2D-V2 path in docs/setup/linux.md." >&2
  echo "Set LEGACY_STARTER_STACK_BUILD=1 if you intentionally maintain the old autotools build." >&2
  exit 1
fi

cd Lib
make -j8
make install
cd ../Agent
make -j8
