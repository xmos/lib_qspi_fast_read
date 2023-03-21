
// Copyright (c) 2023 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public License: Version 1

/* System headers */
#include <platform.h>
#include <string.h>
#include <xs1.h>
#include <xcore/hwtimer.h>
#include <quadflashlib.h>

#include "qspi_flash_fast_read.h"
#include "unity.h"
#include "unity_fixture.h"

#if SYS_FREQ == 600
#define SPEED_MAX   50.0f
#elif SYS_FREQ == 800
#define SPEED_MAX   66.5f
#else
#error Invalid System Frequency
#endif

extern qspi_fast_flash_read_ctx_t qspi_fast_flash_read_ctx;
extern qspi_fast_flash_read_ctx_t *ctx;

TEST_GROUP(qspi_flash_fast_read_stress_thruput_div3);

TEST_SETUP(qspi_flash_fast_read_stress_thruput_div3)
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
        3
    );
    qspi_flash_fast_read_setup_resources(ctx);
    qspi_flash_fast_read_calibrate(ctx, addr, qspi_flash_fast_read_pattern_expect_default, scratch_buf, QFFR_DEFAULT_CAL_PATTERN_BUF_SIZE_WORDS);
    qspi_flash_fast_read_apply_calibration(ctx);
}

TEST_TEAR_DOWN(qspi_flash_fast_read_stress_thruput_div3)
{
    qspi_flash_fast_read_shutdown(ctx);
}

__attribute__((fptrgroup("unity_test_func")))
TEST(qspi_flash_fast_read_stress_thruput_div3, test_qspi_flash_fast_read_data)
{
    uint8_t buf[250000];

    uint32_t start = get_reference_time();
    qspi_flash_fast_read(ctx, 0, buf, (size_t)250000);
    uint32_t end = get_reference_time();
    float thruput = 250000 * 100. / (end-start);

    /* Within 5% of Theoretical max MB/s */
    TEST_ASSERT_FLOAT_WITHIN(SPEED_MAX * 0.05, SPEED_MAX, thruput);
    TEST_PRINTF("Thruput was %f\n", thruput);
}

__attribute__((fptrgroup("unity_test_func")))
TEST(qspi_flash_fast_read_stress_thruput_div3, test_qspi_flash_fast_read_data_nibble_swap)
{
    uint8_t buf[250000];
    qspi_flash_fast_read_mode_set(ctx, qspi_fast_flash_read_transfer_nibble_swap);
    
    uint32_t start = get_reference_time();
    qspi_flash_fast_read(ctx, 0, buf, (size_t)250000);
    uint32_t end = get_reference_time();
    float thruput = 250000 * 100. / (end-start);

    /* Nibbleswapping is slower, heavily dependant on len since swapping must occur
     * outside of the critical IO loop */
    TEST_ASSERT_FLOAT_WITHIN(SPEED_MAX * 0.15, SPEED_MAX / 2 ,thruput);
    TEST_PRINTF("Thruput was %f\n", thruput);
}


TEST_GROUP_RUNNER(qspi_flash_fast_read_stress_thruput_div3) {
    RUN_TEST_CASE(qspi_flash_fast_read_stress_thruput_div3, test_qspi_flash_fast_read_data);
    RUN_TEST_CASE(qspi_flash_fast_read_stress_thruput_div3, test_qspi_flash_fast_read_data_nibble_swap);
}
