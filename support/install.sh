#!/bin/bash

while [ -n "$1" ]; do
  case "$1" in
    -u)
      using_build_cache="1"
      ;;
    --target)
      target="$2"
      shift
      ;;
    *)
      echo "error: unknown option $1"
      ;;
  esac
  shift
done

if [ -z "$using_build_cache" ]; then
  rm -rf build install || exit $?
fi

mkdir -p build || exit $?

cmake_task="-B ./build -S ."
build_task="./build"
install_task="-DCMAKE_INSTALL_PREFIX=/ -P ./build/cmake_install.cmake"

if [ -n "$target" ]; then
  build_task="$build_task --target $target"
  install_task="-DCOMPONENT=$target $install_task"
fi

bash -x -c "cmake $cmake_task" || exit $?
bash -x -c "cmake --build $build_task" || exit $?
bash -x -c "DESTDIR=./install cmake $install_task" || exit $?

exit 0

