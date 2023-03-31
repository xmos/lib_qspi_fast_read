#!/bin/bash
# Copyright (c) 2023, XMOS Ltd, All rights reserved

set -e
set -x

# help text
help()
{
    echo "QSPI test builder"
    echo
    echo "Syntax: build_tests.sh [-h]"
    echo
    echo "options:"
    echo "h     Print this Help."
}

# discern repository root
LIB_QSPI_FAST_READ_ROOT=`git rev-parse --show-toplevel`

START_DIR=$PWD
cd ${LIB_QSPI_FAST_READ_ROOT}/build
make test_general
make test_resource_reuse
make test_stress_600
make test_stress_800

cd ${LIB_QSPI_FAST_READ_ROOT}/test/legacy_build_test
xmake
cd ${START_DIR}
