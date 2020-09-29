#!/bin/bash

rm -rf build install

mkdir build

cmake -B ./build -S .
if $?
  return $?

cmake --build ./build
if $?
  return $?

DESTDIR=./install cmake -DCMAKE_INSTALL_PREFIX=/ -P ./build/cmake_install.cmake
if $?
  return $?

export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:$(pwd)/install/support/

