// Copyright (c) 2023 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public License: Version 1

/* System headers */
#include <platform.h>
#include <stdio.h>
#include <xs1.h>
#include <xcore/clock.h>
#include <xcore/port.h>
#include <xcore/hwtimer.h>
#include <xcore/parallel.h>
#include <xclib.h>

#include "qspi_flash_fast_read.h"
#include "unity.h"
#include "unity_fixture.h"

DECLARE_JOB(test, (void));
DECLARE_JOB(dummy, (void));

qspi_fast_flash_read_ctx_t qspi_fast_flash_read_ctx;
qspi_fast_flash_read_ctx_t *ctx = &qspi_fast_flash_read_ctx;

asm ("  \
.weak  _fptrgroup.unity_test_func.nstackwords.group;    \
.max_reduce _fptrgroup.unity_test_func.nstackwords, _fptrgroup.unity_test_func.nstackwords.group, 0;    \
.set   UnityTestRunner.nstackwords, _fptrgroup.unity_test_func.nstackwords; \
.globl UnityTestRunner.nstackwords; \
");

void test(void)
{
    RUN_TEST_GROUP(qspi_flash_fast_read_valid_div3);
    RUN_TEST_GROUP(qspi_flash_fast_read_valid_div6);
    RUN_TEST_GROUP(qspi_flash_fast_read_stress_thruput_div3);
    RUN_TEST_GROUP(qspi_flash_fast_read_stress_thruput_div6);

    UnityEnd();

    _Exit((int)Unity.TestFailures);
}

int main(int argc, const char* argv[])
{
    UnityGetCommandLineOptions(argc, argv);
    UnityBegin(argv[0]);

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
