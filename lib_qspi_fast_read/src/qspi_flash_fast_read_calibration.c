// Copyright (c) 2023 XMOS LIMITED. This Software is subject to the terms of the
// XMOS Public License: Version 1

#include "qspi_flash_fast_read.h"

uint32_t qspi_flash_fast_read_pattern_expect_default[QFFR_DEFAULT_CAL_PATTERN_BUF_SIZE_WORDS] = {
    0x0f0f00ff,
    0x0f0f0f0f,
    0x00ff00ff,
    0x00ff00ff,
    0x08cef731,
    0x08cef731,
    0x639c639c,
    0x639c639c
};
