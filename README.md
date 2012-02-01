# HipHop for PHP

HipHop is a source code transformer which transforms PHP source code into highly optimized C++ and then compiles it using g++. Currently supported platforms are Linux and FreeBSD. There is no OS X support.

* [Developer Mailing List](http://groups.google.com/group/hiphop-php-dev)
* [Wiki](http://wiki.github.com/facebook/hiphop-php)
* [Issue Tracker](http://github.com/facebook/hiphop-php/issues)

## Required Packages

The latest information is available on the [wiki](http://wiki.github.com/facebook/hiphop-php/building-and-installing)

* cmake *2.6 is the minimum version*
* g++/gcc *4.3 is the minimum version*
* Boost *1.37 is the minimum version*
* flex
* bison
* re2c
* libmysql
* libxml2
* libmcrypt
* libicu *4.2 is the minimum version*
* openssl
* binutils
* libcap
* gd
* zlib
* tbb *Intel's Thread Building Blocks*
* [Oniguruma](http://www.geocities.jp/kosako3/oniguruma/)
* libpcre
* libexpat
* libmemcached

The following packages have had slight modifications added to them. Patches are provided and should be made against the current source copies.

* [libcurl](http://curl.haxx.se/download.html)
* src/third_party/libcurl.fb-changes.diff
* [libevent 1.4](http://www.monkey.org/~provos/libevent/)
* src/third_party/libevent-1.4.13.fb-changes.diff	OR src/third_party/libevent-1.4.14.fb-changes.diff

## Installation

You may need to point CMake to the location of your custom libcurl and libevent, or to any other libraries which needed to be installed. The *CMAKE_PREFIX_PATH* variable is used to hint to the location.

    export CMAKE_PREFIX_PATH=/home/user

To build HipHop, use the following:

Linux:

    cd /home/user/dev
    git clone git://github.com/facebook/hiphop-php.git
    cd hiphop-php
    git submodule init
    git submodule update
    export HPHP_HOME=`pwd`
    export HPHP_LIB=`pwd`/bin
    cmake .

If you are using FreeBSD instead use export - setenv

Once this is done you can generate the build file. This will return you to the shell. Finally, to build, run `make`. If any errors occur, it may be required to remove the CMakeCache.txt directory in the checkout.

    make

## Contributing to HipHop
HipHop is licensed under the PHP and Zend licenses except as otherwise noted.

Before changes can be accepted a [Contributors Licensing Agreement](http://developers.facebook.com/opensource/cla) must be signed and returned.

## Running HipHop

Please see [the wiki page](http://wiki.github.com/facebook/hiphop-php/running-hiphop)

## Generated files

When updating your modificated branch with github version, you will enter in a
eternal pain because of the automatically generated source code files. You will
have also problem with the merge.

Your best bet will be to revert automatically generated files to the github
version to do the first compilation. This is the list of the files you probably
will need to revert:

src/system/gen/php/classes/exception.cpp
src/system/gen/php/classes/exception.h
src/system/gen/sys/dynamic_table_class.cpp
src/system/gen/sys/dynamic_table_constant.cpp
src/system/gen/sys/dynamic_table_func.no.cpp
src/system/gen/sys/literal_strings.h
src/system/gen/sys/literal_strings_0.no.cpp
src/system/gen/sys/system_globals.h
