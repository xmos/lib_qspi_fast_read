#!/bin/bash
# Copyright (c) 2023, XMOS Ltd, All rights reserved

set -e
set -x

# help text
help()
{
    echo "QSPI hardware configuration installer"
    echo
    echo "Syntax: config_hardware_str_50.sh [-h] adapterID_optional"
    echo
    echo "options:"
    echo "h     Print this Help."
}

# flag arguments
while getopts h option
do
    case "${option}" in
        h) help
           exit;;
    esac
done

# assign command line args
if [ ! -z "${@:$OPTIND:1}" ]
then
    ADAPTER_ID="--adapter-id ${@:$OPTIND:1}"
fi

# discern repository root
LIB_QSPI_FAST_READ_ROOT=`git rev-parse --show-toplevel`

cd ${LIB_QSPI_FAST_READ_ROOT}; rm -rf build
cd ${LIB_QSPI_FAST_READ_ROOT}; mkdir build
cd ${LIB_QSPI_FAST_READ_ROOT}/build; cmake ../ -DLIB_QSPI_FAST_READ_TESTS=ON -DCMAKE_TOOLCHAIN_FILE=xmos_cmake_toolchain/xs3a.cmake

make flash_config_drive_str_50_ci_test
# flash for testing
make flash_erase_ci_tests
make flash_data_ci_tests