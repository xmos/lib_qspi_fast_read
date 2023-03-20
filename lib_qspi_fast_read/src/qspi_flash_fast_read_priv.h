// Copyright (c) 2023 XMOS LIMITED. This Software is subject to the terms of the
// XMOS Public License: Version 1

#ifndef QSPI_FAST_FLASH_READ_PRIV_H_
#define QSPI_FAST_FLASH_READ_PRIV_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Defines for SETPADCTRL
#define DR_STR_2mA     0
#define DR_STR_4mA     1
#define DR_STR_8mA     2
#define DR_STR_12mA    3

// Set the drive strength to 8mA on all ports.
// This will reduce rise time uncertainty and ensure ports will fully switch at 100MHz rate over PVT. (Remember our corner silicon was only skewed for core transistors not IO).
// Setting all ports to the same drive to ensure we don't introduce any extra skew between outputs.
// Downside is extra EMI.
#define PORT_PAD_CTL_SMT    0           // Schmitt off
#define PORT_PAD_CTL_SR     1           // Fast slew
#ifndef PORT_PAD_CTL_DR_STR
#define PORT_PAD_CTL_DR_STR DR_STR_8mA  // 8mA drive
#endif
#define PORT_PAD_CTL_REN    1           // Receiver enabled
#define PORT_PAD_CTL_MODE   0x0006

#define QSPI_FF_PORT_PAD_CTL ((PORT_PAD_CTL_SMT     << 23) | \
                              (PORT_PAD_CTL_SR      << 22) | \
                              (PORT_PAD_CTL_DR_STR  << 20) | \
                              (PORT_PAD_CTL_REN     << 17) | \
                              (PORT_PAD_CTL_MODE    << 0))

/* Pad delay uses mode bits 0x7007 with the value stored in bits 11..3 
 * Can only take on the values 0-4 */
#define QSPI_FF_SETC_PAD_DELAY(n) (0x7007 | ((n) << 3))

__attribute__((always_inline))
inline uint32_t qspi_ff_nibble_swap(uint32_t word)
{
	uint32_t tmp;
    asm volatile (
		"{and %0,%0,%2 ; and  %1,%0,%3}\n"
		"{shl %0,%0,4  ; shr  %1,%1,4}\n"
		: "+r"(word), "=r"(tmp)
		: "r"(0x0F0F0F0F), "r"(0xF0F0F0F0)
	);

	return word | tmp;
}

#ifdef __cplusplus
};
#endif

#endif /* QSPI_FAST_FLASH_READ_PRIV_H_ */
