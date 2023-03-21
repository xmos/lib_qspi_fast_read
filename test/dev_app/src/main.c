// Copyright (c) 2023 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public License: Version 1

/* System headers */
#include <stdio.h>
#include <stdlib.h>
#include <platform.h>
#include <xs1.h>
#include <xcore/clock.h>
#include <xcore/port.h>
#include <xcore/hwtimer.h>
#include <xcore/parallel.h>
#include <xclib.h>
#include "qspi_flash_fast_read.h"

DECLARE_JOB(test, (void));
DECLARE_JOB(dummy, (void));

#define CLK_DIVIDE 3

qspi_fast_flash_read_ctx_t qspi_fast_flash_read_ctx;
qspi_fast_flash_read_ctx_t *ctx = &qspi_fast_flash_read_ctx;

static void check(uint32_t addr, size_t size)
{
    unsigned read_data[10240];
    uint32_t start = get_reference_time();
    qspi_flash_fast_read(ctx, addr, (uint8_t*)read_data, size);
    uint32_t end = get_reference_time();
    printf("duration: %lu bytes: %d start: %lu end: %lu\n", end-start, size, start, end);

    char *ptr = (char*)&read_data;
    for(int i=0; i<size; i++) {
        printf("%02x ", (char)*(ptr+i));
    }
    printf("\n");
}


void test(void)
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

    if (qspi_flash_fast_read_calibrate(ctx, addr, qspi_flash_fast_read_pattern_expect_default, scratch_buf, QFFR_DEFAULT_CAL_PATTERN_BUF_SIZE_WORDS) != 0) {
        printf("Fast flash calibration failed\n");
        _Exit(0);
    }

    qspi_flash_fast_read_mode_set(ctx, qspi_fast_flash_read_transfer_nibble_swap);

    printf("Test 1\n");
    check(0x100000, 10240);

    qspi_flash_fast_read_shutdown(ctx);
    qspi_flash_fast_read_setup_resources(ctx);
    qspi_flash_fast_read_apply_calibration(ctx);

    printf("Test 2\n");
    check(0x100000, 10);

    qspi_flash_fast_read_shutdown(ctx);
    qspi_flash_fast_read_setup_resources(ctx);
    qspi_flash_fast_read_apply_calibration(ctx);

    printf("Test 3\n");
    check(0x100000, 1);

    printf("Test 4\n");
    check(0x100001, 8);

    _Exit(0);
}

// Dummy thread to make sure we're running 8 threads.
void dummy(void)
{
    volatile int i = 0;
    volatile int j = 0;

    while(1)
    {
      i++;
      if (i == 0) {
        j = i;
      }
    }
}

int main(void)
{
    PAR_JOBS (
        PJOB(test, ()),
        PJOB(dummy, ()),
        PJOB(dummy, ()),
        PJOB(dummy, ()),
        PJOB(dummy, ()),
        PJOB(dummy, ()),
        PJOB(dummy, ()),
        PJOB(dummy, ())
    );
    return 0;
}
