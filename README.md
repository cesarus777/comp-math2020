[![CodeFactor](https://www.codefactor.io/repository/github/cesarus777/comp-math2020/badge)](https://www.codefactor.io/repository/github/cesarus777/comp-math2020)

# **comp-math2020**
Solving problems and laboratory practice from Computational Mathematics course

## Short description
**lab1** - comparison analytical derivative with one to four-order numerical derivatieves

### Dependencies
To build and run you need
- C++ compiler supporting c\++2a/c\++20 (e.g. g++\-8)
- cmake 3.13.4 or newer
- gnuplot, tested only with 5.2 version


## Build and install
#### Manually
You can build and install either using cmake manually:
```bash
mkdir -p build
cmake -B build -S .
cmake --build build
DESTDIR=./install cmake -DCMAKE_INSTALL_PREFIX=/ -P build/cmake_install.cmake
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:`pwd`/install/support
```

#### Automatically
Or use support script:
```bash
chmod +x install_support.sh
. install_support.sh
```

## Run
After installation you can find binaries and scripts in `install/` directory.

Usage information about each one you can find in corresponding READMEs:
- [lab1](lab1/README.md)
