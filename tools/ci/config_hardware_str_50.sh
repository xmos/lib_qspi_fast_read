#!/bin/bash
# Copyright (c) 2023, XMOS Ltd, All rights reserved

set -e
set -x

# help text
help()
{
    echo "QSPI hardware configuration installer. Sets drive strength on flash to 50%"
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

# 0x06 - write enable, 0x11 0x00 0x40 sets drive strength to 50% (from default 25%) on the Winbond flash
xflash ${ADAPTER_ID} --spi-command 0x06 --target-file=${LIB_QSPI_FAST_READ_ROOT}/test/ci/XK_VOICE_L71.xn
xflash ${ADAPTER_ID} --spi-command 0x11 0 0x40 --target-file=${LIB_QSPI_FAST_READ_ROOT}/test/ci/XK_VOICE_L71.xn
# flash for testing
xflash ${ADAPTER_ID} --erase-all --target-file=${LIB_QSPI_FAST_READ_ROOT}/test/ci/XK_VOICE_L71.xn
xflash ${ADAPTER_ID} --write-all ${LIB_QSPI_FAST_READ_ROOT}/test/ci/ci_test_flash_contents.bin --target-file=${LIB_QSPI_FAST_READ_ROOT}/test/ci/XK_VOICE_L71.xn
