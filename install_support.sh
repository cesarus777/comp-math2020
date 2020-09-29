#!/bin/bash

rm -rf build install

mkdir build

cmake -B ./build -S .
if $?
then
  return $?
fi

cmake --build ./build
if $?
then
  return $?
fi

DESTDIR=./install cmake -DCMAKE_INSTALL_PREFIX=/ -P ./build/cmake_install.cmake
if $?
then
  return $?
fi

export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:$(pwd)/install/support/

