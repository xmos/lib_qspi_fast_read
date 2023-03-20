// Copyright (c) 2023 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public License: Version 1

/* System headers */
#include <platform.h>
#include <xs1.h>
#include <xcore/clock.h>
#include <xcore/port.h>
#include <xcore/hwtimer.h>
#include <xcore/parallel.h>
#include <xclib.h>

#include "qspi_flash_fast_read.h"
#include "unity.h"

#include "api_coverage/api_coverage.h"

qspi_fast_flash_read_ctx_t qspi_fast_flash_read_ctx;
qspi_fast_flash_read_ctx_t *ctx = &qspi_fast_flash_read_ctx;

void setUp(void) {
    ;
}

void tearDown(void) {
    ;
}

int main(void)
{
    UNITY_BEGIN();
    test_qspi_flash_fast_read_init();
    test_qspi_flash_fast_read_shutdown();
    test_qspi_flash_fast_read_mode_set();
    test_qspi_flash_fast_read_mode_get();
    test_qspi_flash_fast_read_setup_resources();
    test_qspi_flash_fast_read_calibrate();
    test_qspi_flash_fast_read_apply_calibration();
    test_qspi_flash_fast_read();

    return UNITY_END();
}
