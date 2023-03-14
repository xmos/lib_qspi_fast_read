# Fast Flash Thruput Eval

This is a fast flash example application to evaluate flash performance. By default this application configures for 133 MHz SCLK.

## Supported Hardware

This example is supported on the XK_VOICE_L71 board.

## Building the Firmware

Run the following commands in the root folder to build the firmware:

On Linux and Mac run:

    cmake -B build -DCMAKE_TOOLCHAIN_FILE=xmos_cmake_toolchain/xs3a.cmake
    cd build
    make example_ff_thruput_eval

On Windows run:

    cmake -G "NMake Makefiles" -B build -D CMAKE_TOOLCHAIN_FILE=xmos_cmake_toolchain/xs3a.cmake
    cd build
    nmake example_ff_thruput_eval

## Running the Firmware

Before the firmware is run, the flash calibration pattern must be loaded. Run the following commands from the build folder.

On Linux and Mac run:

    make flash_calibration_example_ff_thruput_eval

On Windows run:

    nmake flash_calibration_example_ff_thruput_eval

Once flashed, the application can be run.

On Linux and Mac run:

    make run_example_ff_thruput_eval

On Windows run:

    nmake run_example_ff_thruput_eval

## Debugging the firmware with `xgdb`

Run the following commands in the build folder.

On Linux and Mac run:

    make debug_example_ff_thruput_eval

On Windows run:

    nmake debug_example_ff_thruput_eval
