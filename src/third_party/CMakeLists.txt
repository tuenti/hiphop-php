include(ExternalProject)


ExternalProject_Add(
    curl
    URL http://curl.haxx.se/download/curl-7.26.0.tar.gz
    PATCH_COMMAND patch -p1 <  ${CMAKE_SOURCE_DIR}/src/third_party/libcurl.diff
    INSTALL_DIR ${CMAKE_BINARY_DIR}/external_build/
    CONFIGURE_COMMAND <SOURCE_DIR>/configure --prefix=<INSTALL_DIR> --prefix=<INSTALL_DIR> --without-librtmp --with-ca-path=/etc/ssl/certs --with-lber-lib=lber --with-gssapi=/usr --enable-ares --disable-ipv6 -disable-symbol-hiding --enable-versioned-symbols
)

ExternalProject_Add(
    libevent
    URL http://cloud.github.com/downloads/libevent/libevent/libevent-2.0.16-stable.tar.gz
    PATCH_COMMAND patch -p1 <  ${CMAKE_SOURCE_DIR}/src/third_party/libevent-2.0.16-changes.diff
    INSTALL_DIR ${CMAKE_BINARY_DIR}/external_build/
    CONFIGURE_COMMAND <SOURCE_DIR>/configure --prefix=<INSTALL_DIR> --prefix=<INSTALL_DIR>
)

ExternalProject_Add(
    graphicsmagick
    URL http://downloads.sourceforge.net/project/graphicsmagick/graphicsmagick/1.3.15/GraphicsMagick-1.3.15.tar.gz
    PATCH_COMMAND patch -p1 <  ${CMAKE_SOURCE_DIR}/src/third_party/graphicsmagick-1.3.13-changes.diff
    INSTALL_DIR ${CMAKE_BINARY_DIR}/external_build/
    CONFIGURE_COMMAND <SOURCE_DIR>/configure --prefix=<INSTALL_DIR> --disable-openmp --enable-static=no --enable-shared=yes
)

ExternalProject_Add(
    libmemcached
    GIT_REPOSITORY git://github.com/dpaneda/libmemcached.git
    INSTALL_DIR ${CMAKE_BINARY_DIR}/external_build/
    CONFIGURE_COMMAND <SOURCE_DIR>/configure --disable-sasl --prefix=<INSTALL_DIR>
)

ExternalProject_Add(
    jemalloc
    URL http://www.canonware.com/download/jemalloc/jemalloc-3.0.0.tar.bz2
    INSTALL_DIR ${CMAKE_BINARY_DIR}/external_build/
    CONFIGURE_COMMAND <SOURCE_DIR>/configure --prefix=<INSTALL_DIR>
)

if (NOT SKIP_BUNDLED_XHP)
    add_subdirectory(xhp/xhp)
endif()

add_subdirectory(libafdt)
add_subdirectory(libmbfl)
add_subdirectory(libsqlite3)
add_subdirectory(timelib)

add_dependencies(afdt libevent)
