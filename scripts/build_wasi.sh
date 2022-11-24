#!/bin/bash

# Install latest version of wasienv
pip install wasix

mkdir -p out
cd out

# Make the regular build, wrapping it with wasimake
wasix-make cmake .. -DCMAKE_BUILD_TYPE=Release -DWASI=ON
make -j 8
