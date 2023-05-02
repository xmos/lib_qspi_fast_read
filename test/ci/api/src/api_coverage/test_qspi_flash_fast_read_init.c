
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

TEST_GROUP(qspi_flash_fast_read_init);

TEST_SETUP(qspi_flash_fast_read_init) {}

TEST_TEAR_DOWN(qspi_flash_fast_read_init) {}

TEST(qspi_flash_fast_read_init, test_qspi_flash_fast_read_init_transfer_mode_raw)
{
    qspi_flash_fast_read_init(ctx,
        XS1_CLKBLK_1,
        XS1_PORT_1B,
        XS1_PORT_1C,
        XS1_PORT_4B,
        qspi_fast_flash_read_transfer_raw,
        3
    );

    TEST_ASSERT_EQUAL_UINT32(XS1_CLKBLK_1, ctx->clock_block);
    TEST_ASSERT_EQUAL_UINT32(XS1_PORT_1B, ctx->cs_port);
    TEST_ASSERT_EQUAL_UINT32(XS1_PORT_1C, ctx->sclk_port);
    TEST_ASSERT_EQUAL_UINT32(XS1_PORT_4B, ctx->sio_port);
    TEST_ASSERT_EQUAL_UINT32(qspi_fast_flash_read_transfer_raw, ctx->mode);
    TEST_ASSERT_EQUAL_UINT8(3, ctx->divide);
    TEST_ASSERT_EQUAL_UINT32(0, ctx->read_start_pt);
    TEST_ASSERT_EQUAL_UINT32(0, ctx->sdelay);
    TEST_ASSERT_EQUAL_UINT32(0, ctx->pad_delay);
}

TEST(qspi_flash_fast_read_init, test_qspi_flash_fast_read_init_transfer_mode_nibble_swap)
{
    qspi_flash_fast_read_init(ctx,
        XS1_CLKBLK_1,
        XS1_PORT_1B,
        XS1_PORT_1C,
        XS1_PORT_4B,
        qspi_fast_flash_read_transfer_nibble_swap,
        3
    );

    TEST_ASSERT_EQUAL_UINT32(XS1_CLKBLK_1, ctx->clock_block);
    TEST_ASSERT_EQUAL_UINT32(XS1_PORT_1B, ctx->cs_port);
    TEST_ASSERT_EQUAL_UINT32(XS1_PORT_1C, ctx->sclk_port);
    TEST_ASSERT_EQUAL_UINT32(XS1_PORT_4B, ctx->sio_port);
    TEST_ASSERT_EQUAL_UINT32(qspi_fast_flash_read_transfer_nibble_swap, ctx->mode);
    TEST_ASSERT_EQUAL_UINT8(3, ctx->divide);
    TEST_ASSERT_EQUAL_UINT32(0, ctx->read_start_pt);
    TEST_ASSERT_EQUAL_UINT32(0, ctx->sdelay);
    TEST_ASSERT_EQUAL_UINT32(0, ctx->pad_delay);
}

TEST(qspi_flash_fast_read_init, test_qspi_flash_fast_read_init_transfer_mode_arbitrary)
{
    qspi_flash_fast_read_init(ctx,
        XS1_CLKBLK_1,
        XS1_PORT_1B,
        XS1_PORT_1C,
        XS1_PORT_4B,
        71793,
        3
    );

    TEST_ASSERT_EQUAL_UINT32(XS1_CLKBLK_1, ctx->clock_block);
    TEST_ASSERT_EQUAL_UINT32(XS1_PORT_1B, ctx->cs_port);
    TEST_ASSERT_EQUAL_UINT32(XS1_PORT_1C, ctx->sclk_port);
    TEST_ASSERT_EQUAL_UINT32(XS1_PORT_4B, ctx->sio_port);
    TEST_ASSERT_EQUAL_UINT32(qspi_fast_flash_read_transfer_raw, ctx->mode);
    TEST_ASSERT_EQUAL_UINT8(3, ctx->divide);
    TEST_ASSERT_EQUAL_UINT32(0, ctx->read_start_pt);
    TEST_ASSERT_EQUAL_UINT32(0, ctx->sdelay);
    TEST_ASSERT_EQUAL_UINT32(0, ctx->pad_delay);
}

TEST(qspi_flash_fast_read_init, test_qspi_flash_fast_read_init_divide_floor)
{
    qspi_flash_fast_read_init(ctx,
        XS1_CLKBLK_1,
        XS1_PORT_1B,
        XS1_PORT_1C,
        XS1_PORT_4B,
        qspi_fast_flash_read_transfer_raw,
        0x00
    );

    TEST_ASSERT_EQUAL_UINT32(XS1_CLKBLK_1, ctx->clock_block);
    TEST_ASSERT_EQUAL_UINT32(XS1_PORT_1B, ctx->cs_port);
    TEST_ASSERT_EQUAL_UINT32(XS1_PORT_1C, ctx->sclk_port);
    TEST_ASSERT_EQUAL_UINT32(XS1_PORT_4B, ctx->sio_port);
    TEST_ASSERT_EQUAL_UINT32(qspi_fast_flash_read_transfer_raw, ctx->mode);
    TEST_ASSERT_EQUAL_UINT8(3, ctx->divide);
    TEST_ASSERT_EQUAL_UINT32(0, ctx->read_start_pt);
    TEST_ASSERT_EQUAL_UINT32(0, ctx->sdelay);
    TEST_ASSERT_EQUAL_UINT32(0, ctx->pad_delay);
}

TEST(qspi_flash_fast_read_init, test_qspi_flash_fast_read_init_divide_ceiling)
{
    qspi_flash_fast_read_init(ctx,
        XS1_CLKBLK_1,
        XS1_PORT_1B,
        XS1_PORT_1C,
        XS1_PORT_4B,
        qspi_fast_flash_read_transfer_raw,
        0xFF
    );

    TEST_ASSERT_EQUAL_UINT32(XS1_CLKBLK_1, ctx->clock_block);
    TEST_ASSERT_EQUAL_UINT32(XS1_PORT_1B, ctx->cs_port);
    TEST_ASSERT_EQUAL_UINT32(XS1_PORT_1C, ctx->sclk_port);
    TEST_ASSERT_EQUAL_UINT32(XS1_PORT_4B, ctx->sio_port);
    TEST_ASSERT_EQUAL_UINT32(qspi_fast_flash_read_transfer_raw, ctx->mode);
    TEST_ASSERT_EQUAL_UINT8(6, ctx->divide);
    TEST_ASSERT_EQUAL_UINT32(0, ctx->read_start_pt);
    TEST_ASSERT_EQUAL_UINT32(0, ctx->sdelay);
    TEST_ASSERT_EQUAL_UINT32(0, ctx->pad_delay);
}

TEST_GROUP_RUNNER(qspi_flash_fast_read_init) {
    RUN_TEST_CASE(qspi_flash_fast_read_init, test_qspi_flash_fast_read_init_transfer_mode_raw);
    RUN_TEST_CASE(qspi_flash_fast_read_init, test_qspi_flash_fast_read_init_transfer_mode_nibble_swap);
    RUN_TEST_CASE(qspi_flash_fast_read_init, test_qspi_flash_fast_read_init_transfer_mode_arbitrary);
    RUN_TEST_CASE(qspi_flash_fast_read_init, test_qspi_flash_fast_read_init_divide_floor);
    RUN_TEST_CASE(qspi_flash_fast_read_init, test_qspi_flash_fast_read_init_divide_ceiling);
}