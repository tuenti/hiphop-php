#!/bin/bash
# Usage: ./build.sh [--packages]
#
# --packages
#     Create debug and non-debug packages

packages=false
(( $# > 0 )) && [ $1 == "--packages" ] && packages=true

export HPHP_HOME=`pwd`
PATH="/usr/lib/ccache:$PATH"
export CC=gcc-4.6
export CXX=g++-4.6

if ! $packages
then
  [ ! -d build ] && mkdir build
  cd build
  cmake ..
  make -j 4
else
  # installing needed deps
  sudo apt-get update
  sudo apt-get install `cat build_dependencies`
  rm -rf build && mkdir build && cd build
  cmake -DCMAKE_BUILD_TYPE=Debug ..
  make -j 4 package
  cd ..
  mv build/*deb .
  rm -rf build && mkdir build && cd build
  cmake ..
  make -j 4 package
  cd ..
  mv build/*deb .
fi
