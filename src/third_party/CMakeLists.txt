include(ExternalProject)


ExternalProject_Add(
    curl
    URL http://curl.haxx.se/download/curl-7.25.0.tar.gz
    PATCH_COMMAND patch -p1 <  ${CMAKE_SOURCE_DIR}/src/third_party/libcurl.fb-changes.diff
    INSTALL_DIR ${CMAKE_BINARY_DIR}/external_build/
    CONFIGURE_COMMAND <SOURCE_DIR>/configure --prefix=<INSTALL_DIR> --prefix=<INSTALL_DIR>
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

if (NOT SKIP_BUNDLED_XHP)
    add_subdirectory(xhp/xhp)
endif()

add_subdirectory(libafdt)
add_subdirectory(libmbfl)
add_subdirectory(libsqlite3)
add_subdirectory(timelib)

add_dependencies(afdt libevent)
