#!/bin/bash

# installing needed deps
apt-get install libgeoip-dev libgeoip1 libmagic-dev libmagic1

export HPHP_HOME=`pwd`

if [ ! -d build ]; then
	mkdir build
fi

cd build

cmake ..
make -j1 package

echo -e "\n\nDone.\n\n"

ls -la hiphop-debug-*-Linux-*.deb
