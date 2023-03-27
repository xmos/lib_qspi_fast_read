// Copyright (c) 2023 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public License: Version 1

/* System headers */
#include <platform.h>
#include <xs1.h>
#include <stdio.h>
#include <stdlib.h>
#include <xcore/clock.h>
#include <xcore/port.h>
#include <xcore/hwtimer.h>
#include <xcore/parallel.h>
#include <xclib.h>

#include "qspi_flash_fast_read.h"

DECLARE_JOB(thruput, (void));
DECLARE_JOB(dummy, (void));

#ifdef XCORE_AI_EXPLORER
#define CLK_DIVIDE          4
#elif XK_VOICE_L71
#define CLK_DIVIDE          3
#else
#error Board not supported
#endif
#define THRUPUT_SIZE_BYTES  250000 /* 1/4 MB */
#define THRUPUT_ADDR        0x00000000
#define LOOPS_BEFORE_EXIT   10

uint32_t read_data_words[THRUPUT_SIZE_BYTES/4];

qspi_fast_flash_read_ctx_t qspi_fast_flash_read_ctx;
qspi_fast_flash_read_ctx_t *ctx = &qspi_fast_flash_read_ctx;

void thruput(void)
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

    if (qspi_flash_fast_read_calibrate(ctx, addr, qspi_flash_fast_read_pattern_expect_default, scratch_buf, QFFR_DEFAULT_CAL_PATTERN_BUF_SIZE_WORDS) != 0) {
        printf("Fast flash calibration failed\n");
        _Exit(-1);
    }

    uint8_t *read_data = (uint8_t*)read_data_words;

    for(int i=0; i<LOOPS_BEFORE_EXIT; i++) {
        uint32_t start = get_reference_time();
        qspi_flash_fast_read(ctx, THRUPUT_ADDR, read_data, THRUPUT_SIZE_BYTES);
        uint32_t end = get_reference_time();
        printf("Thruput %f MB/s\n", THRUPUT_SIZE_BYTES * 100. / (end-start));
    }

    qspi_flash_fast_read_shutdown(ctx);

    _Exit(0);
}

/* Dummy thread to fully utilize pipeline stages */
void dummy(void)
{
    volatile int i = 0;
    volatile int j = 0;

    while(1) {
      i++;
      if (i == 0) {
        j = i;
      }
    }
}

int main(void)
{
    PAR_JOBS (
        PJOB(thruput, ()),
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
