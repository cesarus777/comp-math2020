#!/bin/bash

rm -rf build install

mkdir build

while true; do
  cmake -B ./build -S .
  if $?; then
    break
  fi

  cmake --build ./build
  if $?; then
    break
  fi

  DESTDIR=./install cmake -DCMAKE_INSTALL_PREFIX=/ -P ./build/cmake_install.cmake
  if $?; then
    break
  fi

  break
done

export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:$(pwd)/install/support/

