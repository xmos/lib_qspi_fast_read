// Copyright (c) 2023 XMOS LIMITED. This Software is subject to the terms of the
// XMOS Public License: Version 1

#ifndef QSPI_FAST_FLASH_READ_H_
#define QSPI_FAST_FLASH_READ_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <xs1.h>
#include <xclib.h>
#include <xcore/clock.h>
#include <xcore/port.h>

typedef enum {
	qspi_fast_flash_read_transfer_raw = 0, /**< Do not bit reverse port in */
	qspi_fast_flash_read_transfer_nibble_swap   /**< Nibble swap port ins */
} qspi_fast_flash_read_transfer_mode_t;

typedef struct {
	xclock_t clock_block;
	port_t cs_port;
	port_t sclk_port;
	port_t sio_port;
    qspi_fast_flash_read_transfer_mode_t mode;
    uint8_t divide;
	
	uint32_t read_start_pt;
} qspi_fast_flash_read_ctx_t;

// Define the clock source divide - 600MHz/800MHz core clock divided by (2*CLK_DIVIDE)
// CORE CLOCK   600MHz    800MHz
// CLK_DIVIDE      SPI_CLK
// 3            100MHz    133MHz
// 4            75MHz     100MHz
// 5            60MHz     80MHz
// 6            50MHz     66MHz


// Only supports flash parts with:
// 0xEB Fast Quad read instruction where address and data bits are input and output over QSPI
// Flash read command is of the format:
// instruction 8 cycles
// 3 bytes addr
// 6 dummy (may be 2 mode 4 dummy, note mode will implicitly be set to 0)
// data read

void qspi_flash_fast_read_init(
    qspi_fast_flash_read_ctx_t *ctx,
	xclock_t clock_block,
	port_t cs_port,
	port_t sclk_port,
	port_t sio_port,
    qspi_fast_flash_read_transfer_mode_t mode,
    uint8_t divide);

void qspi_flash_fast_read(
    qspi_fast_flash_read_ctx_t *ctx,
	uint32_t addr,
	uint8_t *buf,
	size_t len);

/* Returns -1 on failure */
int32_t qspi_flash_fast_read_calibrate(
    qspi_fast_flash_read_ctx_t *ctx,
	uint32_t addr,
	uint32_t *expect_buf,
	uint32_t *scratch_buf,
	size_t len_words);
	
void qspi_flash_fast_read_setup_resources(
    qspi_fast_flash_read_ctx_t *ctx);

void qspi_flash_fast_read_shutdown(
    qspi_fast_flash_read_ctx_t *ctx);

#define QFFR_DEFAULT_CAL_PATTERN_BUF_SIZE_WORDS     8

extern uint32_t qspi_flash_fast_read_pattern_expect_default[QFFR_DEFAULT_CAL_PATTERN_BUF_SIZE_WORDS];
 
#ifdef __cplusplus
};
#endif

#endif /* QSPI_FAST_FLASH_READ_H_ */
