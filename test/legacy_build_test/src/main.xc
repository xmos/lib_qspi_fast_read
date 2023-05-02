// Copyright (c) 2023 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public License: Version 1

#include <platform.h>
#include <xs1.h>
#include <xclib.h>
#include <xscope.h>

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

#include "app.h"

unsafe{

int main(void) {
    par {
      on tile[0]: {
        thruput();
      }
    }

  return 0;
}

} /* unsafe */
