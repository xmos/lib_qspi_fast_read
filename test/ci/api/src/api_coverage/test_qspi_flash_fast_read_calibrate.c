
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

TEST_GROUP(qspi_flash_fast_read_calibrate);

TEST_SETUP(qspi_flash_fast_read_calibrate) {}

TEST_TEAR_DOWN(qspi_flash_fast_read_calibrate) {}

TEST(qspi_flash_fast_read_calibrate, test_qspi_flash_fast_read_calibrate_pass)
{
    // Setup the clock block and ports
    qspi_flash_fast_read_init(ctx,
        XS1_CLKBLK_1,
        XS1_PORT_1B,
        XS1_PORT_1C,
        XS1_PORT_4B,
        qspi_fast_flash_read_transfer_raw,
        CLK_DIVIDE
    );
    qspi_flash_fast_read_setup_resources(ctx);

    uint32_t addr = 0x00000000;
    uint32_t scratch_buf[QFFR_DEFAULT_CAL_PATTERN_BUF_SIZE_WORDS];

    TEST_ASSERT_EQUAL_INT32(0, qspi_flash_fast_read_calibrate(ctx, addr, qspi_flash_fast_read_pattern_expect_default, scratch_buf, QFFR_DEFAULT_CAL_PATTERN_BUF_SIZE_WORDS));
}

TEST(qspi_flash_fast_read_calibrate, test_qspi_flash_fast_read_calibrate_nibble_swap_pass)
{
    // Setup the clock block and ports
    qspi_flash_fast_read_init(ctx,
        XS1_CLKBLK_1,
        XS1_PORT_1B,
        XS1_PORT_1C,
        XS1_PORT_4B,
        qspi_fast_flash_read_transfer_nibble_swap,
        CLK_DIVIDE
    );
    qspi_flash_fast_read_setup_resources(ctx);

    uint32_t addr = 0x00001000;
    uint32_t scratch_buf[QFFR_DEFAULT_CAL_PATTERN_BUF_SIZE_WORDS];

    TEST_ASSERT_EQUAL_INT32(0, qspi_flash_fast_read_calibrate(ctx, addr, qspi_flash_fast_read_pattern_expect_default, scratch_buf, QFFR_DEFAULT_CAL_PATTERN_BUF_SIZE_WORDS));
}

TEST(qspi_flash_fast_read_calibrate, test_qspi_flash_fast_read_calibrate_fail)
{
    /* Configure to nibble swap to result in calibration failure */
    qspi_flash_fast_read_init(ctx,
        XS1_CLKBLK_1,
        XS1_PORT_1B,
        XS1_PORT_1C,
        XS1_PORT_4B,
        qspi_fast_flash_read_transfer_nibble_swap,
        CLK_DIVIDE
    );
    qspi_flash_fast_read_setup_resources(ctx);

    uint32_t addr = 0x00000000;
    uint32_t scratch_buf[QFFR_DEFAULT_CAL_PATTERN_BUF_SIZE_WORDS];

    TEST_ASSERT_NOT_EQUAL_INT32(0, qspi_flash_fast_read_calibrate(ctx, addr, qspi_flash_fast_read_pattern_expect_default, scratch_buf, QFFR_DEFAULT_CAL_PATTERN_BUF_SIZE_WORDS));
}

TEST(qspi_flash_fast_read_calibrate, test_qspi_flash_fast_read_calibrate_invalid_expect_buf)
{
    /* Configure to nibble swap to result in calibration failure */
    qspi_flash_fast_read_init(ctx,
        XS1_CLKBLK_1,
        XS1_PORT_1B,
        XS1_PORT_1C,
        XS1_PORT_4B,
        qspi_fast_flash_read_transfer_nibble_swap,
        CLK_DIVIDE
    );
    qspi_flash_fast_read_setup_resources(ctx);

    uint32_t addr = 0x00000000;
    uint32_t scratch_buf[QFFR_DEFAULT_CAL_PATTERN_BUF_SIZE_WORDS];

    TEST_ASSERT_NOT_EQUAL_INT32(0, qspi_flash_fast_read_calibrate(ctx, addr, NULL, scratch_buf, QFFR_DEFAULT_CAL_PATTERN_BUF_SIZE_WORDS));
}

TEST(qspi_flash_fast_read_calibrate, test_qspi_flash_fast_read_calibrate_invalid_scratch_buf)
{
    /* Configure to nibble swap to result in calibration failure */
    qspi_flash_fast_read_init(ctx,
        XS1_CLKBLK_1,
        XS1_PORT_1B,
        XS1_PORT_1C,
        XS1_PORT_4B,
        qspi_fast_flash_read_transfer_nibble_swap,
        CLK_DIVIDE
    );
    qspi_flash_fast_read_setup_resources(ctx);

    uint32_t addr = 0x00000000;

    TEST_ASSERT_NOT_EQUAL_INT32(0, qspi_flash_fast_read_calibrate(ctx, addr, qspi_flash_fast_read_pattern_expect_default, NULL, QFFR_DEFAULT_CAL_PATTERN_BUF_SIZE_WORDS));
}

TEST_GROUP_RUNNER(qspi_flash_fast_read_calibrate) {
    RUN_TEST_CASE(qspi_flash_fast_read_calibrate, test_qspi_flash_fast_read_calibrate_pass);
    RUN_TEST_CASE(qspi_flash_fast_read_calibrate, test_qspi_flash_fast_read_calibrate_nibble_swap_pass);
    RUN_TEST_CASE(qspi_flash_fast_read_calibrate, test_qspi_flash_fast_read_calibrate_fail);
    RUN_TEST_CASE(qspi_flash_fast_read_calibrate, test_qspi_flash_fast_read_calibrate_invalid_expect_buf);
    RUN_TEST_CASE(qspi_flash_fast_read_calibrate, test_qspi_flash_fast_read_calibrate_invalid_scratch_buf);

}
