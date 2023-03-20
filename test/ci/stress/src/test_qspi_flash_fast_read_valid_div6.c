
// Copyright (c) 2023 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public License: Version 1

/* System headers */
#include <platform.h>
#include <string.h>
#include <xs1.h>
#include <xcore/clock.h>
#include <xcore/port.h>
#include <quadflashlib.h>

#include "qspi_flash_fast_read.h"
#include "unity.h"
#include "unity_fixture.h"

extern qspi_fast_flash_read_ctx_t qspi_fast_flash_read_ctx;
extern qspi_fast_flash_read_ctx_t *ctx;

TEST_GROUP(qspi_flash_fast_read_valid_div6);

TEST_SETUP(qspi_flash_fast_read_valid_div6)
{
    memset(ctx, 0x00, sizeof(qspi_fast_flash_read_ctx_t));
    uint32_t addr = 0x00000000;
    uint32_t scratch_buf[QFFR_DEFAULT_CAL_PATTERN_BUF_SIZE_WORDS];

    qspi_flash_fast_read_init(ctx,
        XS1_CLKBLK_1,
        XS1_PORT_1B,
        XS1_PORT_1C,
        XS1_PORT_4B,
        qspi_fast_flash_read_transfer_raw,
        6
    );
    qspi_flash_fast_read_setup_resources(ctx);
    qspi_flash_fast_read_calibrate(ctx, addr, qspi_flash_fast_read_pattern_expect_default, scratch_buf, QFFR_DEFAULT_CAL_PATTERN_BUF_SIZE_WORDS);
    qspi_flash_fast_read_apply_calibration(ctx);
}

TEST_TEAR_DOWN(qspi_flash_fast_read_valid_div6)
{
    qspi_flash_fast_read_shutdown(ctx);
}

__attribute__((fptrgroup("unity_test_func")))
TEST(qspi_flash_fast_read_valid_div6, test_qspi_flash_fast_read)
{
    uint32_t buf[QFFR_DEFAULT_CAL_PATTERN_BUF_SIZE_WORDS * 4];


    qspi_flash_fast_read(ctx, 0, (uint8_t*)&buf[0], (size_t)(QFFR_DEFAULT_CAL_PATTERN_BUF_SIZE_WORDS * 4));
    TEST_ASSERT_EQUAL_UINT32_ARRAY(qspi_flash_fast_read_pattern_expect_default, buf, QFFR_DEFAULT_CAL_PATTERN_BUF_SIZE_WORDS);
}

__attribute__((fptrgroup("unity_test_func")))
TEST(qspi_flash_fast_read_valid_div6, test_qspi_flash_fast_read_data)
{
    uint8_t buf[255];
    qspi_flash_fast_read_mode_set(ctx, qspi_fast_flash_read_transfer_nibble_swap);
    
    qspi_flash_fast_read(ctx, 0x10000, buf, (size_t)255);
    
    for(uint8_t i=0; i<255; i++)
    {
        TEST_ASSERT_EQUAL_UINT8(i, buf[i]);
    }
}

__attribute__((fptrgroup("unity_test_func")))
TEST(qspi_flash_fast_read_valid_div6, test_qspi_flash_fast_read_erased)
{
    uint8_t buf[255];

    qspi_flash_fast_read(ctx, 0x100000, buf, (size_t)255);
    
    for(uint8_t i=0; i<255; i++)
    {
        TEST_ASSERT_EQUAL_UINT8(0xFF, buf[i]);  /* Erased should be 0xFF*/
    }
}

TEST_GROUP_RUNNER(qspi_flash_fast_read_valid_div6) {
    RUN_TEST_CASE(qspi_flash_fast_read_valid_div6, test_qspi_flash_fast_read);
    RUN_TEST_CASE(qspi_flash_fast_read_valid_div6, test_qspi_flash_fast_read_data);
    RUN_TEST_CASE(qspi_flash_fast_read_valid_div6, test_qspi_flash_fast_read_erased);
}
