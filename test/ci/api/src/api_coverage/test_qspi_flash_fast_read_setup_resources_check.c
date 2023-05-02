
// Copyright (c) 2023 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public License: Version 1

/* System headers */
#include <platform.h>
#include <xs1.h>

#include "qspi_flash_fast_read.h"
#include "unity.h"
#include "unity_fixture.h"

extern qspi_fast_flash_read_ctx_t qspi_fast_flash_read_ctx;
extern qspi_fast_flash_read_ctx_t *ctx;

TEST_GROUP(qspi_flash_fast_read_setup_resources);

TEST_SETUP(qspi_flash_fast_read_setup_resources) {}

TEST_TEAR_DOWN(qspi_flash_fast_read_setup_resources) {}

TEST(qspi_flash_fast_read_setup_resources, test_qspi_flash_fast_read_setup_resources_check)
{
    qspi_flash_fast_read_init(ctx,
        XS1_CLKBLK_1,
        XS1_PORT_1B,
        XS1_PORT_1C,
        XS1_PORT_4B,
        qspi_fast_flash_read_transfer_raw,
        3
    );

    qspi_flash_fast_read_setup_resources(ctx);

    /* For now just call function to check for gross errors */
}

TEST_GROUP_RUNNER(qspi_flash_fast_read_setup_resources) {
    RUN_TEST_CASE(qspi_flash_fast_read_setup_resources, test_qspi_flash_fast_read_setup_resources_check);
}