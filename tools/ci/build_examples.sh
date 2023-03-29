#!/bin/bash
set -e

XCORE_VOICE_ROOT=`git rev-parse --show-toplevel`

# setup configurations
# row format is: "name   board    toolchain"
examples=(
    "example_ff_thruput_eval    XK_VOICE_L71         xmos_cmake_toolchain/xs3a.cmake"
    "example_ff_thruput_eval    XCORE_AI_EXPLORER    xmos_cmake_toolchain/xs3a.cmake"
)

# perform builds
for ((i = 0; i < ${#examples[@]}; i += 1)); do
    read -ra FIELDS <<< ${examples[i]}
    name="${FIELDS[0]}"
    board="${FIELDS[1]}"
    toolchain_file="${XCORE_VOICE_ROOT}/${FIELDS[2]}"
    app_target="${name}_${board}"

    path="${XCORE_VOICE_ROOT}"

    echo '******************************************************'
    echo '* Building' ${app_target} 'for' ${board}
    echo '******************************************************'

    (cd ${path}; cmake -B build -DCMAKE_TOOLCHAIN_FILE=${toolchain_file} -DBOARD=${board})
    (cd ${path}/build; make ${app_target} -j)
done