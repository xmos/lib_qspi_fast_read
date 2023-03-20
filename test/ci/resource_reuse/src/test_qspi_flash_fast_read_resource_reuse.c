
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

TEST_GROUP(qspi_flash_fast_read_resource_reuse);

TEST_SETUP(qspi_flash_fast_read_resource_reuse) {
    memset(ctx, 0x00, sizeof(qspi_fast_flash_read_ctx_t));
}

TEST_TEAR_DOWN(qspi_flash_fast_read_resource_reuse) {}

TEST(qspi_flash_fast_read_resource_reuse, test_qspi_flash_fast_read_resource_reuse_libquadflash)
{
    uint32_t addr = 0x00000000;
    uint32_t scratch_buf[QFFR_DEFAULT_CAL_PATTERN_BUF_SIZE_WORDS];
    uint32_t buf[QFFR_DEFAULT_CAL_PATTERN_BUF_SIZE_WORDS * 4];

    qspi_flash_fast_read_init(ctx,
        XS1_CLKBLK_1,
        XS1_PORT_1B,
        XS1_PORT_1C,
        XS1_PORT_4B,
        qspi_fast_flash_read_transfer_raw,
        CLK_DIVIDE
    );
    qspi_flash_fast_read_setup_resources(ctx);
    qspi_flash_fast_read_calibrate(ctx, addr, qspi_flash_fast_read_pattern_expect_default, scratch_buf, QFFR_DEFAULT_CAL_PATTERN_BUF_SIZE_WORDS);
    
    /* lib_qspi_flash_fast_read usage */
    {
        qspi_flash_fast_read_apply_calibration(ctx);

        qspi_flash_fast_read(ctx, addr, (uint8_t*)&buf[0], (size_t)(QFFR_DEFAULT_CAL_PATTERN_BUF_SIZE_WORDS * 4));
        TEST_ASSERT_EQUAL_UINT32_ARRAY(qspi_flash_fast_read_pattern_expect_default, buf, QFFR_DEFAULT_CAL_PATTERN_BUF_SIZE_WORDS);

        qspi_flash_fast_read_shutdown(ctx);
    }
    memset((int8_t *)buf, 0x00, QFFR_DEFAULT_CAL_PATTERN_BUF_SIZE_WORDS * 4);

    /* Other library using resources */
    {
        fl_QuadDeviceSpec qspi_spec = FL_QUADDEVICE_DEFAULT;
        fl_QSPIPorts qspi_ports = {
            .qspiCS = XS1_PORT_1B,
            .qspiSCLK = XS1_PORT_1C,
            .qspiSIO = XS1_PORT_4B,
            .qspiClkblk = XS1_CLKBLK_1,
        };

        TEST_ASSERT_EQUAL_INT32(0, fl_connectToDevice(&qspi_ports, &qspi_spec, 1));
        /* Perform some operation that will use the resources */
        TEST_ASSERT_EQUAL_INT32(4096, fl_getSectorSize(0));

        fl_disconnect();
    }

    /* lib_qspi_flash_fast_read re-usage */
    {
        qspi_flash_fast_read_setup_resources(ctx);
        qspi_flash_fast_read_apply_calibration(ctx);
        qspi_flash_fast_read(ctx, addr, (uint8_t*)&buf[0], (size_t)(QFFR_DEFAULT_CAL_PATTERN_BUF_SIZE_WORDS * 4));
        TEST_ASSERT_EQUAL_UINT32_ARRAY(qspi_flash_fast_read_pattern_expect_default, buf, QFFR_DEFAULT_CAL_PATTERN_BUF_SIZE_WORDS);

        fl_disconnect();
    }
}

TEST(qspi_flash_fast_read_resource_reuse, test_qspi_flash_fast_read_resource_reuse_general)
{
    uint32_t addr = 0x00000000;
    uint32_t scratch_buf[QFFR_DEFAULT_CAL_PATTERN_BUF_SIZE_WORDS];
    uint32_t buf[QFFR_DEFAULT_CAL_PATTERN_BUF_SIZE_WORDS * 4];

    qspi_flash_fast_read_init(ctx,
        XS1_CLKBLK_1,
        XS1_PORT_1B,
        XS1_PORT_1C,
        XS1_PORT_4B,
        qspi_fast_flash_read_transfer_raw,
        CLK_DIVIDE
    );
    qspi_flash_fast_read_setup_resources(ctx);
    qspi_flash_fast_read_calibrate(ctx, addr, qspi_flash_fast_read_pattern_expect_default, scratch_buf, QFFR_DEFAULT_CAL_PATTERN_BUF_SIZE_WORDS);
    
    /* lib_qspi_flash_fast_read usage */
    {
        qspi_flash_fast_read_apply_calibration(ctx);

        qspi_flash_fast_read(ctx, addr, (uint8_t*)&buf[0], (size_t)(QFFR_DEFAULT_CAL_PATTERN_BUF_SIZE_WORDS * 4));
        TEST_ASSERT_EQUAL_UINT32_ARRAY(qspi_flash_fast_read_pattern_expect_default, buf, QFFR_DEFAULT_CAL_PATTERN_BUF_SIZE_WORDS);
        qspi_flash_fast_read_shutdown(ctx);
    }
    memset((int8_t *)buf, 0x00, QFFR_DEFAULT_CAL_PATTERN_BUF_SIZE_WORDS * 4);

    /* Use resources */
    {
        clock_enable(XS1_CLKBLK_1);
        clock_set_source_clk_ref(XS1_CLKBLK_1);
        clock_set_divide(XS1_CLKBLK_1, 1);
        clock_start(XS1_CLKBLK_1);

        port_enable(XS1_PORT_1B);
        port_enable(XS1_PORT_1C);
        port_enable(XS1_PORT_4B);
        
        /* Do some arbitrary things, but remember we are still
         * physically attached to a qspi_flash part
         * so we cannot be too crazy */
        port_set_trigger_time(XS1_PORT_1B, PORT_TIMESTAMP_MAX);

        port_set_invert(XS1_PORT_1C);
        (void) port_in(XS1_PORT_1C);

        port_set_buffered(XS1_PORT_4B);
        port_set_transfer_width(XS1_PORT_4B, 8);
        port_set_sample_falling_edge(XS1_PORT_4B);

        (void) port_in(XS1_PORT_4B);

        port_disable(XS1_PORT_1B);
        port_disable(XS1_PORT_1C);
        port_disable(XS1_PORT_4B);

        clock_stop(XS1_CLKBLK_1);
        clock_disable(XS1_CLKBLK_1);
    }

    /* lib_qspi_flash_fast_read re-usage */
    {
        qspi_flash_fast_read_setup_resources(ctx);
        qspi_flash_fast_read_apply_calibration(ctx);
        qspi_flash_fast_read(ctx, addr, (uint8_t*)&buf[0], (size_t)(QFFR_DEFAULT_CAL_PATTERN_BUF_SIZE_WORDS * 4));
        TEST_ASSERT_EQUAL_UINT32_ARRAY(qspi_flash_fast_read_pattern_expect_default, buf, QFFR_DEFAULT_CAL_PATTERN_BUF_SIZE_WORDS);

        fl_disconnect();
    }
}

TEST_GROUP_RUNNER(qspi_flash_fast_read_resource_reuse) {
    RUN_TEST_CASE(qspi_flash_fast_read_resource_reuse, test_qspi_flash_fast_read_resource_reuse_libquadflash);
    RUN_TEST_CASE(qspi_flash_fast_read_resource_reuse, test_qspi_flash_fast_read_resource_reuse_general);
}
