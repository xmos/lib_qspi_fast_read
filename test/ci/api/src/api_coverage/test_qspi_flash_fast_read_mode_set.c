
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

TEST_GROUP(qspi_flash_fast_read_mode_set);

TEST_SETUP(qspi_flash_fast_read_mode_set) {}

TEST_TEAR_DOWN(qspi_flash_fast_read_mode_set) {}

TEST(qspi_flash_fast_read_mode_set, test_qspi_flash_fast_read_mode_set_check)
{
    qspi_flash_fast_read_init(ctx,
        XS1_CLKBLK_1,
        XS1_PORT_1B,
        XS1_PORT_1C,
        XS1_PORT_4B,
        qspi_fast_flash_read_transfer_raw,
        3
    );

    qspi_flash_fast_read_mode_set(ctx, qspi_fast_flash_read_transfer_nibble_swap);
    TEST_ASSERT_EQUAL_UINT32(qspi_fast_flash_read_transfer_nibble_swap, ctx->mode);

    qspi_flash_fast_read_mode_set(ctx, qspi_fast_flash_read_transfer_raw);
    TEST_ASSERT_EQUAL_UINT32(qspi_fast_flash_read_transfer_raw, ctx->mode);
}

TEST(qspi_flash_fast_read_mode_set, test_qspi_flash_fast_read_mode_set_invalid_value)
{
    qspi_flash_fast_read_init(ctx,
        XS1_CLKBLK_1,
        XS1_PORT_1B,
        XS1_PORT_1C,
        XS1_PORT_4B,
        qspi_fast_flash_read_transfer_raw,
        3
    );

    qspi_flash_fast_read_mode_set(ctx, 0xFF);
    
    /* Verify mode was unchanged */
    TEST_ASSERT_EQUAL_UINT32(qspi_fast_flash_read_transfer_raw, ctx->mode);
}

TEST_GROUP_RUNNER(qspi_flash_fast_read_mode_set) {
    RUN_TEST_CASE(qspi_flash_fast_read_mode_set, test_qspi_flash_fast_read_mode_set_check);
    RUN_TEST_CASE(qspi_flash_fast_read_mode_set, test_qspi_flash_fast_read_mode_set_invalid_value);
}