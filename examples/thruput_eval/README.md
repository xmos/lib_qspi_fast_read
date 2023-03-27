# Fast Flash Thruput Eval

This is a fast flash example application to evaluate flash performance. By default this application configures for the highest possible SCLK on the targetted hardware.

## Supported Hardware

This example is supported on the XK_VOICE_L71 and XCORE_AI_EXPLORER boards.

On the XK_VOICE_L71 with Winbond W25Q64JW, SCLK is configured to 100 MHz, thus achieving a maximum theoretical thruput of 50 MB/s.

On the XCORE_AI_EXPLORER board with Adesto AT25FF321A, SCLK is configured for 75MHz, thus achieving a maximum theoretical thruput of 37.5 MB/s.

## Building the Firmware

Run the following commands in the root folder to build the firmware, populating the target hardware, ex example_ff_thruput_eval_XCORE_AI_EXPLORER:

On Linux and Mac run:

    cmake -B build -DCMAKE_TOOLCHAIN_FILE=xmos_cmake_toolchain/xs3a.cmake
    cd build
    make example_ff_thruput_eval_[BOARD]

On Windows run:

    cmake -G "NMake Makefiles" -B build -D CMAKE_TOOLCHAIN_FILE=xmos_cmake_toolchain/xs3a.cmake
    cd build
    nmake example_ff_thruput_eval_[BOARD]

## Running the Firmware

Before the firmware is run, the flash calibration pattern must be loaded. Run the following commands from the build folder.

On Linux and Mac run:

    make flash_calibration_example_ff_thruput_eval_[BOARD]

On Windows run:

    nmake flash_calibration_example_ff_thruput_eval_[BOARD]

## XK_VOICE_L71 Only

The Winbond W25Q64JW flash part requires a drive strength increase to maximize data valid duration at high frequency SCLKs. This only needs to be done once as it will write to the nonvolatile config.

On Linux and Mac run:

    make flash_config_drive_str_50_example_ff_thruput_eval

On Windows run:

    nmake flash_config_drive_str_50_example_ff_thruput_eval

To revert back to factory defaults on the IQ package of 25% drive strengh, use the following.

On Linux and Mac run:

    make flash_config_drive_str_25_example_ff_thruput_eval

On Windows run:

    nmake flash_config_drive_str_25_example_ff_thruput_eval

To check the value of status register 3 in the W25Q64JW, use the following.

On Linux and Mac run:

    make flash_config_read_drive_str_example_ff_thruput_eval

On Windows run:

    nmake flash_config_read_drive_str_example_ff_thruput_eval

Once flashed and configured, the application can be run.

## Running the Firmware Continued

On Linux and Mac run:

    make run_example_ff_thruput_eval_[BOARD]

On Windows run:

    nmake run_example_ff_thruput_eval_[BOARD]

## Debugging the firmware with `xgdb`

Run the following commands in the build folder.

On Linux and Mac run:

    make debug_example_ff_thruput_eval_[BOARD]

On Windows run:

    nmake debug_example_ff_thruput_eval_[BOARD]
