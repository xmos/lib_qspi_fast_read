#!/bin/bash
# Copyright (c) 2023, XMOS Ltd, All rights reserved

set -e
set -x

# help text
help()
{
    echo "QSPI fast read tests runner"
    echo
    echo "Syntax: run_tests.sh [-h] adapterID_optional"
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

START_DIR=$PWD
cd ${LIB_QSPI_FAST_READ_ROOT}/build
make run_test_general
make run_test_resource_reuse
make run_test_stress_600
make run_test_stress_800
cd ${START_DIR}
