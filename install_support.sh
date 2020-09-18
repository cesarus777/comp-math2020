#!/bin/bash

rm -rf build install

mkdir build

cmake -B ./build -S .
cmake --build ./build
DESTDIR=./install cmake -DCMAKE_INSTALL_PREFIX=/ -P ./build/cmake_install.cmake

export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:`pwd`/install/support/

