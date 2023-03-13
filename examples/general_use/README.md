# Fast Flash Test

This is a fast flash test application

## Supported Hardware

This example is supported on the XK_VOICE_L71 board.

## Building the Firmware

Run the following commands in the root folder to build the firmware:

On Linux and Mac run:

    cmake -B build -DCMAKE_TOOLCHAIN_FILE=xmos_cmake_toolchain/xs3a.cmake
    cd build
    make example_fast_flash_test

On Windows run:

    cmake -G "NMake Makefiles" -B build -D CMAKE_TOOLCHAIN_FILE=xmos_cmake_toolchain/xs3a.cmake
    cd build
    nmake example_fast_flash_test

## Running the Firmware

Before the firmware is run, the data partition containing the filesystem and
model(s) must be loaded. Run the following commands from the build folder.

On Linux and Mac run:

    make flash_app_example_fast_flash_test

On Windows run:

    nmake flash_app_example_fast_flash_test

Once flashed, the application will run.

If changes are made to the data partition components, the application must be
re-flashed.

If there are no changes to the data partition, run the following from the build
folder.

On Linux and Mac run:

    make run_example_fast_flash_test

On Windows run:

    nmake run_example_fast_flash_test

## Debugging the firmware with `xgdb`

Run the following commands in the build folder.

On Linux and Mac run:

    make debug_example_fast_flash_test

On Windows run:

    nmake debug_example_fast_flash_test
