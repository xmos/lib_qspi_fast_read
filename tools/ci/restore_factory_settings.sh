#!/bin/bash
# Copyright (c) 2023, XMOS Ltd, All rights reserved

set -e
set -x

# help text
help()
{
    echo "Factory drive strength settings restorer"
    echo
    echo "Syntax: restore_factory_settings.sh [-h] adapterID_optional"
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

xflash --spi-command 0x06 --target-file=${LIB_QSPI_FAST_READ_ROOT}/test/ci/XK_VOICE_L71.xn ${ADAPTER_ID}
xflash --spi-command 0x11 0 0x60 --target-file=${LIB_QSPI_FAST_READ_ROOT}/test/ci/XK_VOICE_L71.xn ${ADAPTER_ID}
# TODO verify output 0x60
xflash --spi-command 0x15 1 --target-file=${LIB_QSPI_FAST_READ_ROOT}/test/ci/XK_VOICE_L71.xn ${ADAPTER_ID}
