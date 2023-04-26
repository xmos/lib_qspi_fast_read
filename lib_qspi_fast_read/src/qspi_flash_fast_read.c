// Copyright (c) 2023 XMOS LIMITED. This Software is subject to the terms of the
// XMOS Public License: Version 1

#include <xs1.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xcore/assert.h>

#include "qspi_flash_fast_read.h"
#include "qspi_flash_fast_read_priv.h"

#ifndef READ_ADJUST_MIN_START_CYCLE
/* This is the min clock cycles for valid data to be read
 * 8 clocks for instruction (0xEB)
 * 8 clocks for output addr and mode (currently implicitly 0)
 * @18 port turns around to begin shift register fill
 * 8 clocks for 32bit shift register to be filled 4 bits/clk
 * Meaning on clock 27 would be the first cycle the full
 * transfer is available in the transfer register is available
 * Data will be overwritten in clock 27+8
 * 
 * Allow to be user overridden for advanced users */
#define READ_ADJUST_MIN_START_CYCLE     27
#endif

#ifndef IO_SWITCH_TO_INPUT_CYCLE
/* At specified value of port timer we read the transfer reg word
 * and discard, data will be junk. Exact timing of port going
 * high-z would need simulation but it will be in the cycle specified. */
#define IO_SWITCH_TO_INPUT_CYCLE    18
#endif

/* Max supported clock divider */
#define MAX_CLK_DIVIDE 6


void qspi_flash_fast_read_setup_resources(
    qspi_fast_flash_read_ctx_t *ctx)
{
    clock_enable(ctx->clock_block);
    port_enable(ctx->sclk_port);
    port_enable(ctx->cs_port);
    port_enable(ctx->sio_port);

    clock_set_source_clk_xcore(ctx->clock_block);
    clock_set_divide(ctx->clock_block, ctx->divide);
    port_set_clock(ctx->sclk_port, ctx->clock_block);
    port_set_out_clock(ctx->sclk_port);

    /* Port initial setup */
    port_out(ctx->cs_port, 1);
    port_set_clock(ctx->sio_port, XS1_CLKBLK_REF);
    port_out(ctx->sio_port, 0xF);
	port_sync(ctx->sio_port);
    port_set_clock(ctx->sio_port, ctx->clock_block);
    port_set_buffered(ctx->sio_port);
    port_set_transfer_width(ctx->sio_port, 32);

    port_write_control_word(ctx->sclk_port, QSPI_FF_PORT_PAD_CTL);
    port_write_control_word(ctx->sio_port, QSPI_FF_PORT_PAD_CTL);
    port_write_control_word(ctx->cs_port, QSPI_FF_PORT_PAD_CTL);
    ctx->is_setup = 1;
}

void qspi_flash_fast_read_init(
    qspi_fast_flash_read_ctx_t *ctx,
	xclock_t clock_block,
	port_t cs_port,
	port_t sclk_port,
	port_t sio_port,
    qspi_fast_flash_read_transfer_mode_t mode,
    unsigned char divide)
{
    memset(ctx, 0x00, sizeof(qspi_fast_flash_read_ctx_t));
    ctx->clock_block = clock_block;
    ctx->cs_port = cs_port;
    ctx->sclk_port = sclk_port;
    ctx->sio_port = sio_port;
    
    if (   (mode == qspi_fast_flash_read_transfer_raw) 
        || (mode == qspi_fast_flash_read_transfer_nibble_swap)) {
        ctx->mode = mode;
    } else {
        ctx->mode = qspi_fast_flash_read_transfer_raw;
    }
    
    if (divide < 3) {
        ctx->divide = 3;
    } else if (divide > 6) {
        ctx->divide = 6;
    } else {
        ctx->divide = divide;
    }
}

void qspi_flash_fast_read_shutdown(
    qspi_fast_flash_read_ctx_t *ctx)
{
    if (ctx->is_setup != 0) {
        port_write_control_word(ctx->sio_port, QSPI_FF_SETC_PAD_DELAY(0));

        clock_disable(ctx->clock_block);
        port_disable(ctx->sclk_port);
        port_disable(ctx->cs_port);
        port_disable(ctx->sio_port);
        ctx->is_setup = 0;
    }
}

void qspi_flash_fast_read_apply_calibration(
    qspi_fast_flash_read_ctx_t *ctx)
{
    if (ctx->sdelay  == 1) {
        port_set_sample_falling_edge(ctx->sio_port);
    } else {
        port_set_sample_rising_edge(ctx->sio_port);
    }
    
    port_write_control_word(ctx->sio_port, QSPI_FF_SETC_PAD_DELAY(ctx->pad_delay));
    // printf("Best settings: read adj %lu, sdelay = %lu, pad_delay = %lu\n", ctx->read_start_pt, ctx->sdelay, ctx->pad_delay);
}

int32_t qspi_flash_fast_read_calibrate(
    qspi_fast_flash_read_ctx_t *ctx,
	uint32_t addr,
	uint32_t *expect_buf,
	uint32_t *scratch_buf,
	size_t len_words)
{
    
    if ((expect_buf == NULL) || (scratch_buf == NULL)) {
        return -1;
    }
    
    int32_t retval = 0;

    int32_t pass_start = -1;
    int32_t pass_count = 0;
    int32_t passing_words = 0;
    uint8_t time_index = 0;

    // Bit 0 sdelay, bits 1-2 read adj, bits 3-5 pad delay
    // the index into the array becomes the nominal time.
    uint8_t results[6 * (MAX_CLK_DIVIDE)];

    // This loops over the settings in such a way that the result is sequentially increasing in time in units of core clocks.
    for(int32_t read_adj = 0; read_adj < 3; read_adj++) // Data read port time adjust
    {
        ctx->read_start_pt = READ_ADJUST_MIN_START_CYCLE + read_adj;
        for(int32_t sdelay = 0; sdelay < 2; sdelay++) // Sample delays
        {
            if (sdelay == 1) {
                port_set_sample_falling_edge(ctx->sio_port);
            } else {
                port_set_sample_rising_edge(ctx->sio_port);
            }
            for(int32_t pad_delay = (ctx->divide - 1); pad_delay >= 0; pad_delay--) // Pad delays (only loop over useful pad delays)
            {
                // Set input pad delay in units of core clocks
                port_write_control_word(ctx->sio_port, QSPI_FF_SETC_PAD_DELAY(pad_delay));

                // Read the data with the current settings
                qspi_flash_fast_read(ctx, addr, (uint8_t*)&scratch_buf[0], len_words * sizeof(uint32_t));
                
                // Check if the data is correct
                passing_words = 0;
                for (int32_t m = 0; m < len_words; m++)
                {
                    // printf("got 0x%08x, expect 0x%08x\n", scratch_buf[m], expect_buf[m]);
                    if (scratch_buf[m] == expect_buf[m]) {
                        passing_words++;
                    }
                }

                // Store the settings
                if (passing_words == len_words) {
                    // printf("%d sdelay: %d pad_delay: %d read_adj: %d\n", time_index, sdelay, pad_delay, read_adj);
                    results[time_index] = sdelay | (read_adj << 1) | (pad_delay << 3);
                    pass_count++;
                    if (pass_start == -1) {
                        pass_start = time_index;
                    }
                    // printf("passed %d start %d cnt %d\n", time_index, pass_start, pass_count);
                }
                time_index++;
            }
        }
    }

    if (pass_count >= 5) {
        uint8_t best_setting = pass_start + (pass_count >> 1); // Pick the middle setting

        uint32_t sdelay = (results[best_setting] & 0x01);
        uint32_t pad_delay = (results[best_setting] & 0x38) >> 3;
        uint32_t read_adj  = (results[best_setting] & 0x06) >> 1;
        ctx->sdelay = sdelay;
        ctx->pad_delay = pad_delay;
        ctx->read_start_pt = READ_ADJUST_MIN_START_CYCLE + read_adj;
        
        qspi_flash_fast_read_apply_calibration(ctx);
    } else {
        // printf("Failed to find valid settings.  Pass count: %d\n", pass_count);
        retval = -1;
    }
    
    return retval;
}

void qspi_flash_fast_read_mode_set(
    qspi_fast_flash_read_ctx_t *ctx,
    qspi_fast_flash_read_transfer_mode_t mode)
{
    if ((mode == qspi_fast_flash_read_transfer_raw)
     || (mode == qspi_fast_flash_read_transfer_nibble_swap)) {   
        ctx->mode = mode;
    }
}
	
qspi_fast_flash_read_transfer_mode_t qspi_flash_fast_read_mode_get(
    qspi_fast_flash_read_ctx_t *ctx)
{
    return ctx->mode;
}

void qspi_flash_fast_read(
    qspi_fast_flash_read_ctx_t *ctx,
	uint32_t addr,
	uint8_t *buf,
	size_t len)
{
    uint32_t *read_data = (uint32_t *)buf;
	size_t word_len = len / sizeof(uint32_t);
    uint32_t addr_mode_wr;
    uint32_t tmp = 0;

    /* Save bytes over word size for later */
    len %= 4;

    /* Shift the address left by 8 bits to align the MSB of address
     * with MSB of the port width. Buffered ports shift right so 
     * LSB first. We need to nibble swap the address as it needs
     * to be output MS nibble first and ports do the opposite */
    addr_mode_wr = qspi_ff_nibble_swap(byterev((addr << 8)));
    
    /* Need to set the first data output bit (MS bit of flash command 0xEB)
     * before we start the clock. */
    port_out_part_word(ctx->sio_port, 0x1, 4);
    clock_start(ctx->clock_block);
    port_sync(ctx->sio_port);
    clock_stop(ctx->clock_block);
    
    port_out(ctx->cs_port, 0);
    
    /* Preload the transfer register in the port with the remainder of
     * the first word. This is the 7 LSB of the 0xEB instruction 
     * We need to OR in the first nibble of addr_mode_wr. */
    port_out(ctx->sio_port, 0x01101011 | ((addr_mode_wr & 0x0000000F) << 28));

    /* Start the clock block running. This starts output of data and
     * resets the port timer to 0 on the clk port. */
    clock_start(ctx->clock_block);

    /* Output the remaining 28 bits of addr_mode_wr */
    port_out_part_word(ctx->sio_port, (addr_mode_wr >> 4), 28);
    
    /* Switch the port to input, discard junk data */
    (void) port_in_at_time(ctx->sio_port, IO_SWITCH_TO_INPUT_CYCLE);
    
    /* Read the initial data word at the calibrated read_start_pt cycle
     * After the first word remainder do not need to be timed */
    port_set_trigger_time(ctx->sio_port, ctx->read_start_pt);

    for (int i = 0; i < word_len; i++) {
        read_data[i] = port_in(ctx->sio_port);
    }

    /* Last read for non word multiple bytes */
    if (len > 0) {
        /* Note: This will shift in a full word but we will drop
         * unwanted bits */
        tmp = port_in(ctx->sio_port);
    }
    
    clock_stop(ctx->clock_block);
    port_out(ctx->cs_port, 1);

    /* At 133 MHz there is not enough time in the inner loop to
     * swap so we must do this post IO despite the cost */
    if (ctx->mode == qspi_fast_flash_read_transfer_nibble_swap) {
        for (int i = 0; i < word_len; i++) {
            read_data[i] = qspi_ff_nibble_swap(read_data[i]);
        }
    }

    /* Finish the non word multiple transfer */
    if (len > 0) {  
		if (ctx->mode == qspi_fast_flash_read_transfer_nibble_swap) {
			tmp = qspi_ff_nibble_swap(tmp);
		}

        buf = (uint8_t *) &read_data[word_len];
		for (int i = 0; i < len; i++) {
			*buf++ = (uint8_t)(tmp & 0xFF);
			tmp >>= 8;
		}
    }
}
