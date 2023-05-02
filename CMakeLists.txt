cmake_minimum_required(VERSION 3.21)

## Disable in-source build.
if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
    message(FATAL_ERROR "In-source build is not allowed! Please specify a build folder.\n\tex:cmake -B build")
endif()

## Project declaration
project(lib_qspi_fast_read)

## Enable languages for project
enable_language(CXX C ASM)

## Project options
option(LIB_QSPI_FAST_READ_TESTS     "Enable lib_qspi_fast_read tests"  OFF)

## Setup a root path
set(LIB_QSPI_FAST_READ_ROOT_PATH ${PROJECT_SOURCE_DIR} CACHE STRING "Root folder of lib_qspi_fast_read in this cmake project tree")

## Add library
add_subdirectory(lib_qspi_fast_read)

## Add tools
add_subdirectory(tools)

## Add top level project targets
if(PROJECT_IS_TOP_LEVEL)
    include(examples/examples.cmake)
endif()

if(LIB_QSPI_FAST_READ_TESTS)
    include(test/tests.cmake)
endif()
