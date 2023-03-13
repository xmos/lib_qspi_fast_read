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

DECLARE_JOB(test, (void));
DECLARE_JOB(dummy, (void));

xclock_t clk_spi = XS1_CLKBLK_1;
port_t p_spi_clk = XS1_PORT_1C;
port_t p_spi_csn = XS1_PORT_1B;
port_t p_spi_dat = XS1_PORT_4B;

#define CLK_DIVIDE 6

qspi_fast_flash_read_ctx_t qspi_fast_flash_read_ctx;
qspi_fast_flash_read_ctx_t *ctx = &qspi_fast_flash_read_ctx;


void test(void)
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

    uint32_t addr = 0x00000000;
    uint32_t scratch_buf[QFFR_DEFAULT_CAL_PATTERN_BUF_SIZE_WORDS];

    if (qspi_flash_fast_read_calibrate(ctx, addr, qspi_flash_fast_read_pattern_expect_default, scratch_buf, QFFR_DEFAULT_CAL_PATTERN_BUF_SIZE_WORDS) != 0) {
        printf("Fast flash calibration failed\n");
        _Exit(0);
    }

    {
        unsigned read_data[10240];
        uint32_t start = get_reference_time();
        qspi_flash_fast_read(ctx, 0x100000, read_data, 10240);
        uint32_t end = get_reference_time();
        printf("duration: %d bytes: %d start: %d end: %d\n", end-start, 10240*4, start, end);
        unsigned read_data2[10240];
        for(int i=0; i<10240; i++) {
            read_data2[i] = (read_data[i]);
        }

        char *ptr = &read_data2;
        for(int i=0; i<1024; i++) {
            printf("%02x ", (char)*(ptr+i));
        }
        printf("\n");
    }

    qspi_flash_fast_read_shutdown(ctx);
    qspi_flash_fast_read_setup_resources(ctx);

    {
        printf("Test 2\n");
        unsigned read_data[10240];
        uint32_t start = get_reference_time();
        qspi_flash_fast_read(ctx, 0x100000, read_data, 10);
        uint32_t end = get_reference_time();
        printf("duration: %d bytes: %d start: %d end: %d\n", end-start, 10, start, end);
        unsigned read_data2[10];
        for(int i=0; i<10; i++) {
            read_data2[i] = (read_data[i]);
        }

        char *ptr = &read_data2;
        for(int i=0; i<10; i++) {
            printf("%02x ", (char)*(ptr+i));
        }
        printf("\n");
    }

    qspi_flash_fast_read_shutdown(ctx);
    qspi_flash_fast_read_setup_resources(ctx);

    {
        printf("Test 3\n");
        unsigned read_data[1];
        uint32_t start = get_reference_time();
        qspi_flash_fast_read(ctx, 0x100000, read_data, 1);
        uint32_t end = get_reference_time();
        printf("duration: %d bytes: %d start: %d end: %d\n", end-start, 1, start, end);
        unsigned read_data2[1];
        for(int i=0; i<1; i++) {
            read_data2[i] = (read_data[i]);
        }

        char *ptr = &read_data2;
        for(int i=0; i<1; i++) {
            printf("%02x ", (char)*(ptr+i));
        }
        printf("\n");
    }

    while(1) {
        ; // trap
    }
}

// Dummy thread to make sure we're running 8 threads.
void dummy(void)
{
    int i,j;

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
