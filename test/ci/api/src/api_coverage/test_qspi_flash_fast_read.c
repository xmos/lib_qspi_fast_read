
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

TEST_GROUP(qspi_flash_fast_read);

TEST_SETUP(qspi_flash_fast_read) {}

TEST_TEAR_DOWN(qspi_flash_fast_read) {}

TEST(qspi_flash_fast_read, test_qspi_flash_fast_read_cal_pattern)
{
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

    qspi_flash_fast_read_calibrate(ctx, addr, qspi_flash_fast_read_pattern_expect_default, scratch_buf, QFFR_DEFAULT_CAL_PATTERN_BUF_SIZE_WORDS);
    
    qspi_flash_fast_read_apply_calibration(ctx);
    
    uint32_t buf[QFFR_DEFAULT_CAL_PATTERN_BUF_SIZE_WORDS * 4];

    qspi_flash_fast_read(ctx, addr, (uint8_t*)&buf[0], (size_t)(QFFR_DEFAULT_CAL_PATTERN_BUF_SIZE_WORDS * 4));
    TEST_ASSERT_EQUAL_UINT32_ARRAY(qspi_flash_fast_read_pattern_expect_default, buf, QFFR_DEFAULT_CAL_PATTERN_BUF_SIZE_WORDS);
}

TEST_GROUP_RUNNER(qspi_flash_fast_read) {
    RUN_TEST_CASE(qspi_flash_fast_read, test_qspi_flash_fast_read_cal_pattern);
}