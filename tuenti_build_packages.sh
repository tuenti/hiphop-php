#!/bin/bash

# installing needed deps
sudo apt-get update
sudo apt-get install `cat build_dependencies`

export HPHP_HOME=`pwd`

if [ ! -d build ]; then
	mkdir build
fi

cd build

cmake ..
make -j 4 package
