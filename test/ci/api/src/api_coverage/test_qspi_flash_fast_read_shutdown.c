
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

static void test_qspi_flash_fast_read_shutdown_check(void)
{
    qspi_flash_fast_read_init(ctx,
        XS1_CLKBLK_1,
        XS1_PORT_1B,
        XS1_PORT_1C,
        XS1_PORT_4B,
        qspi_fast_flash_read_transfer_raw,
        3
    );

    qspi_flash_fast_read_shutdown(ctx);

    /* For now just call function to check for gross errors */
}

void test_qspi_flash_fast_read_shutdown(void)
{
    /* Test that appropriate XCORE resources are setup */
    RUN_TEST(test_qspi_flash_fast_read_shutdown_check);
}
