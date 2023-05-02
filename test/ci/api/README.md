# lib_qspi_fast_read API Tests

## Building & running tests

Run the following commands to build the test firmware:

    $ cmake -B build -DLIB_QSPI_FAST_READ_TESTS=ON
    $ cd build
    $ make test_general

Prepare the flash by running the following commands:

    $ make flash_erase_ci_tests
    $ make flash_data_ci_tests

To run the test run:

    $ make run_test_general

## For more unit test options

More advanced uses call  xrun directly instead of using cmake custom targets.

To run a single test group, run with the `-g` option.

    $ xrun --io --args test_general.xe -g {group name}

To run a single test, run with the `-g` and `-n` options.

    $ xrun --io --args test_general.xe -g {group name} -n {test name}

For more unit test options, run with the `-h` option.

    $ xrun --io --args test_general.xe -h
