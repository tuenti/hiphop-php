CMAKE_MINIMUM_REQUIRED(VERSION 2.8.5 FATAL_ERROR)
PROJECT(hphp C CXX)

IF("$ENV{HPHP_HOME}" STREQUAL "")
	message(FATAL_ERROR "You should set the HPHP_HOME environmental")
ENDIF()

file(TO_CMAKE_PATH "$ENV{HPHP_HOME}" HPHP_HOME)

IF(NOT IS_DIRECTORY ${HPHP_HOME})
	message(FATAL_ERROR "The value of HPHP_HOME does not exist")
ENDIF()

IF(NOT EXISTS "${HPHP_HOME}/LICENSE.PHP")
	message(FATAL_ERROR "The value of HPHP_HOME in incorrect")
ENDIF()

SET(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake" ${CMAKE_MODULE_PATH})

include("${CMAKE_CURRENT_SOURCE_DIR}/CMake/HPHPFunctions.cmake")
include(CheckFunctionExists)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)

IF(CMAKE_SIZEOF_VOID_P EQUAL 4)
	message(STATUS "------------")
	message(STATUS "32-bit support is experimental, things may be broken")
	message(STATUS "------------")
ENDIF()

INSTALL(DIRECTORY "${CMAKE_BINARY_DIR}/external_build/lib"
        DESTINATION .
        COMPONENT runtime
        FILES_MATCHING PATTERN "lib*so*"
        PATTERN "Graphi*" EXCLUDE
        PATTERN "pkgconfig" EXCLUDE)

INSTALL(DIRECTORY "${CMAKE_BINARY_DIR}/external_build/include"
        DESTINATION .
        COMPONENT dev
        FILES_MATCHING PATTERN "*.h")

INSTALL(PROGRAMS
        ${CMAKE_BINARY_DIR}/src/hphp/hphpi
        ${CMAKE_BINARY_DIR}/src/hphp/hphp
        ${CMAKE_SOURCE_DIR}/bin/libhphp_runtime.a
        ${CMAKE_SOURCE_DIR}/bin/run.sh
        ${CMAKE_SOURCE_DIR}/bin/CMakeLists.base.txt
        DESTINATION bin
        COMPONENT dev)

INSTALL(DIRECTORY ${CMAKE_SOURCE_DIR}/CMake
        DESTINATION .
        COMPONENT dev)

INSTALL(DIRECTORY
        ${CMAKE_SOURCE_DIR}/src/util
        ${CMAKE_SOURCE_DIR}/src/runtime
        ${CMAKE_SOURCE_DIR}/src/system
        DESTINATION src
        COMPONENT dev
        FILES_MATCHING PATTERN "*.h")

INSTALL(DIRECTORY ${CMAKE_SOURCE_DIR}/src/system
        DESTINATION src
        COMPONENT dev
        FILES_MATCHING PATTERN "*.php")

INSTALL(FILES ${CMAKE_SOURCE_DIR}/LICENSE.PHP
        DESTINATION .
        COMPONENT dev)

INSTALL(FILES ${CMAKE_SOURCE_DIR}/src/runtime/base/memory/smart_allocator.inc_gen
        DESTINATION src/runtime/base/memory
        COMPONENT dev)

SET(CPACK_PACKAGING_INSTALL_PREFIX "/opt/hiphop")
SET(CPACK_DEB_COMPONENT_INSTALL ON)
SET(CPACK_GENERATOR "DEB")
SET(CPACK_DEBIAN_PACKAGE_NAME "hiphop")
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "dpaneda@tuenti.com")

SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "HipHop compiler and runtime dependencies")
SET(CPACK_PACKAGE_VENDOR "Tuenti")
SET(CPACK_PACKAGE_VERSION_MAJOR "0")
SET(CPACK_PACKAGE_VERSION_MINOR "1")
SET(CPACK_PACKAGE_VERSION_PATCH "tuenti")

SET(CPACK_PACKAGE_INSTALL_DIRECTORY "CMake ${CMake_VERSION_MAJOR}.${CMake_VERSION_MINOR}")

INCLUDE(CPack)
