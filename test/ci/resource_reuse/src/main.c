// Copyright (c) 2023 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public License: Version 1

/* System headers */
#include <platform.h>
#include <stdio.h>

#include "qspi_flash_fast_read.h"
#include "unity.h"
#include "unity_fixture.h"

qspi_fast_flash_read_ctx_t qspi_fast_flash_read_ctx;
qspi_fast_flash_read_ctx_t *ctx = &qspi_fast_flash_read_ctx;

int main(int argc, const char* argv[])
{
    UnityGetCommandLineOptions(argc, argv);
    UnityBegin(argv[0]);

    RUN_TEST_GROUP(qspi_flash_fast_read_resource_reuse);

    UnityEnd();

    return (int)Unity.TestFailures;
}
