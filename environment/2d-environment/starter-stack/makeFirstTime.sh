#!/bin/bash
set -e
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

