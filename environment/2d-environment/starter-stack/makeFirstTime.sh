#!/bin/bash
set -e

if [[ "${LEGACY_STARTER_STACK_BUILD:-0}" != "1" ]]; then
  echo "Legacy starter-stack autotools bootstrap is not the supported 2D workflow for this repo." >&2
  echo "Use the CMake-based StarterLibRCSC-V2 + StarterAgent2D-V2 path in LINUX_SETUP.md," >&2
  echo "then run ./link_starteragent2d_v2_compat_2d.sh --force from this directory." >&2
  echo "If you intentionally maintain the legacy build, rerun with LEGACY_STARTER_STACK_BUILD=1." >&2
  exit 1
fi

cd Lib
autoreconf -i
automake --add-missing
./configure CXXFLAGS='-std=c++14 -stdlib=libc++' --prefix=`pwd|sed 's/...$//'`/Agent/Lib
make -j8
make install 
cd ../Agent
autoreconf -i
automake --add-missing
./configure CXXFLAGS="-std=c++14 -stdlib=libc++" --with-librcsc=`pwd`/Lib/
make -j8
