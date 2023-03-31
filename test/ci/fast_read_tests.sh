#!/bin/bash
# Copyright (c) 2023, XMOS Ltd, All rights reserved

set -e
set -x

# help text
help()
{
    echo "QSPI fast read tests"
    echo
    echo "Syntax: fast_read_tests.sh [-h] adapterID_optional"
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

bash ${LIB_QSPI_FAST_READ_ROOT}/tools/ci/config_hardware_str_50.sh ${ADAPTER_ID}
bash ${LIB_QSPI_FAST_READ_ROOT}/tools/ci/build_tests.sh
bash ${LIB_QSPI_FAST_READ_ROOT}/tools/ci/run_tests.sh
bash ${LIB_QSPI_FAST_READ_ROOT}/tools/ci/restore_factory_settings.sh ${ADAPTER_ID}
