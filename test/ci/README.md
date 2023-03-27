# Fast Flash CI Tests

These are the lib_qspi_fast_read CI tests. All sections below must be followed in order.

## Supported Hardware

These tests are supported on the XK_VOICE_L71 with W25Q64JW flash part board. Rev 1V0, Variant A. They may work on other versions, but compatibility needs to be checked for different flash parts.

## Setting Up the Hardware

Run the following commands in the root folder to set up the hardware:

On Linux and Mac run:

    cmake -B build -DLIB_QSPI_FAST_READ_TESTS=ON -DCMAKE_TOOLCHAIN_FILE=xmos_cmake_toolchain/xs3a.cmake
    cd build
    make flash_config_drive_str_50_ci_test

On Windows run:

    cmake -G "NMake Makefiles" -B build -DLIB_QSPI_FAST_READ_TESTS=ON -D CMAKE_TOOLCHAIN_FILE=xmos_cmake_toolchain/xs3a.cmake
    cd build
    nmake flash_config_drive_str_50_ci_test

The test flash contents must be flashed.  These contain the calibration pattern and test data for all tests. Run the following commands in the build folder:

On Linux and Mac run:

    make flash_erase_ci_tests
    make flash_data_ci_tests

On Windows run:

    nmake flash_erase_ci_tests
    nmake flash_data_ci_tests

## Building the Firmware

Run the following commands in the build folder to build the firmware:

On Linux and Mac run:

    make test_general
    make test_resource_reuse
    make test_stress_600
    make test_stress_800
    make legacy_lib_qspi_fast_read_build_test

On Windows run:

    nmake test_general
    nmake test_resource_reuse
    nmake test_stress_600
    nmake test_stress_800
    nmake legacy_lib_qspi_fast_read_build_test

## Running the Firmware

Run the following commands in the build folder to build the firmware:

On Linux and Mac run:

    make run_test_general
    make run_test_resource_reuse
    make run_test_stress_600
    make run_test_stress_800

On Windows run:

    nmake run_test_general
    nmake run_test_resource_reuse
    nmake run_test_stress_600
    nmake run_test_stress_800

Refer to test specific READMES on how to run specific tests. Additionally, all custom cmake targets are just suggestions and the actual xrun calls can be used instead.

## Restoring Default Settings in the Hardware

Run the following commands in the root folder to put the hardware back into default factory condition:

On Linux and Mac run:

    make flash_config_drive_str_25_ci_test

On Windows run:

    nmake flash_config_drive_str_25_ci_test

Verify that the default settings have been restored by calling:

On Linux and Mac run:

    make flash_config_read_drive_str_ci_test

On Windows run:

    nmake flash_config_read_drive_str_ci_test

This will return 0x60 if the default 25% drive strength is restored.
