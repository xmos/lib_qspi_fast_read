
// Copyright (c) 2023 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public License: Version 1

/* System headers */
#include <platform.h>
#include <xs1.h>

#include "qspi_flash_fast_read.h"
#include "unity.h"
#include "api_coverage/api_coverage.h"

extern qspi_fast_flash_read_ctx_t qspi_fast_flash_read_ctx;
extern qspi_fast_flash_read_ctx_t *ctx;

static void test_qspi_flash_fast_read_mode_set_check(void)
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

static void test_qspi_flash_fast_read_mode_set_invalid_value(void)
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

void test_qspi_flash_fast_read_mode_set(void)
{
    RUN_TEST(test_qspi_flash_fast_read_mode_set_check);
    RUN_TEST(test_qspi_flash_fast_read_mode_set_invalid_value);
}
