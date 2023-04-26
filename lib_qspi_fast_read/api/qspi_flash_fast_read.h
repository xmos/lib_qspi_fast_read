// Copyright (c) 2023 XMOS LIMITED. This Software is subject to the terms of the
// XMOS Public License: Version 1

#ifndef QSPI_FAST_FLASH_READ_H_
#define QSPI_FAST_FLASH_READ_H_

#include <stdint.h>
#include <xs1.h>
#include <xclib.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <xcore/clock.h>
#include <xcore/port.h>

/**
 * \addtogroup qspi_flash_fast_read qspi_flash_fast_read
 *
 * The public API for using the qspi_flash_fast_read driver.
 * @{
 */

/**
 * Transfer modes for fast read operations.
 */
typedef enum {
	qspi_fast_flash_read_transfer_raw = 0, /**< Do not nibble swap port ins */
	qspi_fast_flash_read_transfer_nibble_swap /**< Nibble swap port ins */
} qspi_fast_flash_read_transfer_mode_t;

/**
 * Type representing a qspi_flash_fast_read context.
 */
typedef struct qspi_flash_fast_read_struct qspi_fast_flash_read_ctx_t;

/**
 * Struct to hold a qspi_flash_fast_read context.
 *
 * The members in this struct should not be accessed directly.
 */
struct qspi_flash_fast_read_struct {
	/**
	 * The clock block to use for the qspi_io interface.
	 */
	xclock_t clock_block;

	/**
	 * The chip select port. Must be a 1-bit port.
	 */
	port_t cs_port;

	/**
	 * The SCLK port. Must be a 1-bit port.
	 */
	port_t sclk_port;

	/**
	 * The SIO port. Must be a 4-bit port.
	 */
	port_t sio_port;

	/**
	 * The transfer mode for reads to implicitly nibble swap.
	 */
    qspi_fast_flash_read_transfer_mode_t mode;

	/**
	 * The divider to use to generate SCLK.
	 */
    uint8_t divide;

	/* 
	 * The following are used internally and should not modified directly.
	 */
	uint32_t read_start_pt;
	uint32_t sdelay;
    uint32_t pad_delay;
	uint8_t is_setup;
};

/**
 * \addtogroup qspi_flash_fast_read_init qspi_flash_fast_read_init
 *
 * The public API for instantiating the qspi_flash_fast_read device.
 * @{
 */

/**
 * Implements a qspi_flash_fast_read device.
 * 
 * Only supported for use with flash parts with
 * 0xEB Fast Quad read instruction with:
 *   Cycles  Data Transfer   IO
 *   8       0xEB            SPI
 *   6       3 byte address  QSPI
 *   6       Dummy           QSPI (note may be mode byte + 2 dummy bytes)
 *   x       Data read       QSPI
 *
 * \param ctx         A pointer to the qspi_flash_fast_read context to initialize.
 * \param clock_block The clock block that will get configured for use with sclk.
 * \param cs_port     The chip select port. Must be a 1-bit port.
 * \param sclk_port   The SCLK port. Must be a 1-bit port.
 * \param sio_port    The SIO port. Must be a 4-bit port.
 * \param mode        The transfer mode to use for port reads.
 *                    Invalid values will default to qspi_fast_flash_read_transfer_raw
 * \param divide      The divisor to use for QSPI SCLK. Must be between 3 and 6 inclusive.
 *                    SCLK frequency will be set to CORE_CLOCK / (2 * divide)
 *                        CORE CLOCK   600MHz    800MHz
 *                        CLK_DIVIDE      SCLK FREQ
 *                        3            100MHz    133MHz
 *                        4            75MHz     100MHz
 *                        5            60MHz     80MHz
 *                        6            50MHz     66MHz
 *                    Values less than 3 will be implicity set to 3.
 *                    Values greater than 6 will be implicity set to 6
 */
void qspi_flash_fast_read_init(
    qspi_fast_flash_read_ctx_t *ctx,
	xclock_t clock_block,
	port_t cs_port,
	port_t sclk_port,
	port_t sio_port,
    qspi_fast_flash_read_transfer_mode_t mode,
    uint8_t divide);

/**@}*/ // END: addtogroup qspi_flash_fast_read_init

/**
 * Setup and initialize XCORE resources for a qspi_flash_fast_read device.
 * 
 * \param ctx A pointer to the qspi_flash_fast_read context to use.
 */
void qspi_flash_fast_read_setup_resources(
    qspi_fast_flash_read_ctx_t *ctx);

/**
 * Calibrate port delays and timings for a qspi_flash_fast_read device.
 * 
 * This must be called after qspi_flash_fast_read_init() and 
 * qspi_flash_fast_read_setup_resources().
 * 
 * On success qspi_flash_fast_read_apply_calibration() is called implicitly.
 * 
 * \param ctx         A pointer to the qspi_flash_fast_read context to use.
 * \param addr        The address of the calibration pattern.
 * \param expect_buf  A pointer to the byte array with the expected
 *                    calibration pattern.
 * \param scratch_buf A pointer to an application provided scratch buffer.
 *                    This buffer must be at least len_words words.
 *                    Contents will be overwritten. After this function
 *                    call the buffer is owned by the application and no
 *                    longer needed.
 * \param len_words   The length in words of the calibration pattern
 * 
 * \returns    0 on success
 *            -1 on failure
 */
int32_t qspi_flash_fast_read_calibrate(
    qspi_fast_flash_read_ctx_t *ctx,
	uint32_t addr,
	uint32_t *expect_buf,
	uint32_t *scratch_buf,
	size_t len_words);

/**
 * Apply calibration for a qspi_flash_fast_read device.
 * 
 * \param ctx A pointer to the qspi_flash_fast_read context to use.
 */
void qspi_flash_fast_read_apply_calibration(
    qspi_fast_flash_read_ctx_t *ctx);

/**
 * Perform a flash read with a qspi_flash_fast_read device.
 *
 * \param ctx  A pointer to the qspi_flash_fast_read context to use.
 * \param addr The address to read.
 * \param buf  A pointer to the byte array to save the received data.
 *             This must begin on a 4-byte boundary.
 * \param len  The number of bytes to input.
 */
void qspi_flash_fast_read(
    qspi_fast_flash_read_ctx_t *ctx,
	uint32_t addr,
	uint8_t *buf,
	size_t len);

/**
 * Set read mode for a qspi_flash_fast_read device.
 * 
 * \param ctx  A pointer to the qspi_flash_fast_read context to use.
 * \param mode The transfer mode to set.
 */
void qspi_flash_fast_read_mode_set(
    qspi_fast_flash_read_ctx_t *ctx,
    qspi_fast_flash_read_transfer_mode_t mode);

/**
 * Get read mode for a qspi_flash_fast_read device.
 * 
 * \param ctx  A pointer to the qspi_flash_fast_read context to use.
 * 
 * \returns   The current configured transfer mode.
 */
qspi_fast_flash_read_transfer_mode_t qspi_flash_fast_read_mode_get(
    qspi_fast_flash_read_ctx_t *ctx);

/**
 * Shutdown a qspi_flash_fast_read device.
 * 
 * Resets the XCORE resources for application to use.
 * 
 * Calibration is stored internally for future use.
 * 
 * \param ctx A pointer to the qspi_flash_fast_read context to use.
 */
void qspi_flash_fast_read_shutdown(
    qspi_fast_flash_read_ctx_t *ctx);

/**
 * Size in words of the default calibration pattern.
 */
#define QFFR_DEFAULT_CAL_PATTERN_BUF_SIZE_WORDS     8

/**
 * Declaration of array which holds the default calibration pattern.
 * 
 * This pattern was selected to maximize potential EMI and crosstalk.
 */
extern uint32_t qspi_flash_fast_read_pattern_expect_default[QFFR_DEFAULT_CAL_PATTERN_BUF_SIZE_WORDS];
 
/**@}*/ // END: addtogroup qspi_flash_fast_read

#ifdef __cplusplus
};
#endif

#endif /* QSPI_FAST_FLASH_READ_H_ */
