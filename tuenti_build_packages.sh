#!/bin/bash

# installing needed deps
sudo apt-get update
sudo apt-get install `cat build_dependencies`

export HPHP_HOME=`pwd`

if [ ! -d build ]; then
	mkdir build
fi

cd build

export CC=gcc-4.6
export CXX=g++-4.6

cmake ..
make -j 4 package
