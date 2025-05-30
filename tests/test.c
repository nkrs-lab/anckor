/*
 * Copyright (c) 2023 Qoda, engineering
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms and conditions of the GNU General Public License,
 * version 3 or later, as published by the Free Software Foundation.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.

 * You should have received copies of the GNU General Public License and
 * the GNU Lesser General Public License along with this program.  If
 * not, see https://www.gnu.org/licenses/
 */

#include "include/test.h"

#include "app.h"
#include "printf.h"

#define TEST_NUMBER 2

/*******************************************************************************
 * Definitions
 ******************************************************************************/
uint64_t tests_passed = 0;
uint64_t tests_failed = 0;
uint8_t  test_index   = 0;

/******************************************************************************
 * @brief test scheduling routine
 * @param None
 * @return None
 ******************************************************************************/
void main(void) {
  uint64_t test_chan_handler;
  uint64_t test_data     = 0;
  uint64_t test_data_len = 0;
  uint16_t test_id       = 0;
  uint16_t test_flag     = 0;

  printf("ATE - Anckor test engine\r\n");

  // create a channel to receive tests end messages
  ax_channel_create(&test_chan_handler, "test_channel");

  while (test_index < TEST_NUMBER) {
    // block until the thread sends us the TEST_START
    ax_channel_rcv(test_chan_handler, &test_data, &test_data_len);

    // get received data
    test_flag = test_data & BITE_MASK;
    test_id   = (test_data & (BITE_MASK << BITE_SIZE)) >> BITE_SIZE;

    if (test_flag == TEST_START) test_index += 1;

    // if we registered all tasks, wait for their response
    ax_channel_rcv(test_chan_handler, &test_data, &test_data_len);

    test_flag = test_data & BITE_MASK;
    test_id   = (test_data & (BITE_MASK << BITE_SIZE)) >> BITE_SIZE;

    switch (test_flag) {
      case TEST_STOP_OK:
        tests_passed += 1;
        printf("ATE - %x - passed\r\n", test_id);
        break;
      case TEST_STOP_KO:
        tests_failed += 1;
        printf("ATE - %x - failed\r\n", test_id);
        break;
      default:
        printf("ATE rcv erronous value from test application - %x\r\n",
               test_id);
        break;
    }
  }

  // all registered tests have been runned
  if (tests_failed) {
    printf("ATE - FAILED - %d passed - %d failed\r\n", tests_passed,
           tests_failed);
  } else {
    printf("ATE - PASSED - %d passed - %d failed\r\n", tests_passed,
           tests_failed);
  }
}