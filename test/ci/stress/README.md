# lib_qspi_fast_read Stress Tests

## Building & running tests

Run the following commands to build the test firmware:

    $ cmake -B build -DLIB_QSPI_FAST_READ_TESTS=ON
    $ cd build
    $ make test_stress_600
    $ make test_stress_800

There are 2 tests to run:

    $ flash_erase_test_stress_600
    $ make flash_calibration_test_stress_test_600
    $ make test_stress_600

And:

    $ flash_erase_test_stress_800
    $ make flash_calibration_test_stress_test_800
    $ make test_stress_800

## For more unit test options

More advanced uses call  xrun directly instead of using cmake custom targets.

To run a single test group, run with the `-g` option.

    $ xrun --io --args run_test_stress_800.xe -g {group name}

To run a single test, run with the `-g` and `-n` options.

    $ xrun --io --args run_test_stress_800.xe -g {group name} -n {test name}

For more unit test options, run with the `-h` option.

    $ xrun --io --args run_test_stress_800.xe -h
