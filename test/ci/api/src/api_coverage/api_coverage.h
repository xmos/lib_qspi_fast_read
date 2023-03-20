
// Copyright (c) 2023 XMOS LIMITED. This Software is subject to the terms of the
// XMOS Public License: Version 1

#ifndef QSPI_FAST_FLASH_READ_TEST_API_COVERAGE_H_
#define QSPI_FAST_FLASH_READ_TEST_API_COVERAGE_H_

void test_qspi_flash_fast_read_init(void);
void test_qspi_flash_fast_read_setup_resources(void);
void test_qspi_flash_fast_read_calibrate(void);
void test_qspi_flash_fast_read_apply_calibration(void);
void test_qspi_flash_fast_read(void);
void test_qspi_flash_fast_read_mode_set(void);
void test_qspi_flash_fast_read_mode_get(void);
void test_qspi_flash_fast_read_shutdown(void);

#endif /* QSPI_FAST_FLASH_READ_TEST_API_COVERAGE_H_ */
