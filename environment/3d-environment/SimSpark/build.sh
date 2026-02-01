#!/bin/bash
# Simple build and install script

# Build SimSpark
cd spark
mkdir build
cd build
cmake ..
make -j$(nproc)
sudo make install

# Build rcssserver3d
cd ../../rcssserver3d
mkdir build
cd build
cmake ..
make -j$(nproc)
sudo make install
