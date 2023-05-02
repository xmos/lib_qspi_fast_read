
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

TEST_GROUP(qspi_flash_fast_read_mode_get);

TEST_SETUP(qspi_flash_fast_read_mode_get) {}

TEST_TEAR_DOWN(qspi_flash_fast_read_mode_get) {}

TEST(qspi_flash_fast_read_mode_get, test_qspi_flash_fast_read_mode_get_check)
{
    qspi_flash_fast_read_init(ctx,
        XS1_CLKBLK_1,
        XS1_PORT_1B,
        XS1_PORT_1C,
        XS1_PORT_4B,
        qspi_fast_flash_read_transfer_raw,
        3
    );

    TEST_ASSERT_EQUAL_INT32(qspi_fast_flash_read_transfer_raw, qspi_flash_fast_read_mode_get(ctx));

    /* Reach in to modify manually */
    ctx->mode = qspi_fast_flash_read_transfer_nibble_swap;

    TEST_ASSERT_EQUAL_INT32(qspi_fast_flash_read_transfer_nibble_swap, qspi_flash_fast_read_mode_get(ctx));
}

TEST_GROUP_RUNNER(qspi_flash_fast_read_mode_get) {
    RUN_TEST_CASE(qspi_flash_fast_read_mode_get, test_qspi_flash_fast_read_mode_get_check);
}