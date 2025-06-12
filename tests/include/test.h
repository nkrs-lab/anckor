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

#ifndef TEST_H
#define TEST_H

#include "ax_syscall.h"
#include "common.h"

#define _test_section __attribute__((section(".data.tests")))

#define TEST_START   0x000000AA
#define TEST_STOP_OK 0x000000A5
#define TEST_STOP_KO 0x0000005A

/*******************************************************************************
 * macros to use into test applications
 ******************************************************************************/
void test_begin() {
  task_id_t task_id;
  ax_task_get(&task_id);
  uint64_t test_data = (task_id << BITE_SIZE) | TEST_START;
  uint64_t test_chan_handler;
  ax_channel_get(&test_chan_handler, "test_channel");
  ax_channel_snd(test_chan_handler, &test_data, sizeof(test_data));
}

#define TEST_BEGIN() test_begin()

#define TEST_ASSERT(_expr)                                            \
  if (!(_expr)) {                                                     \
    task_id_t task_id;                                                \
    ax_task_get(&task_id);                                            \
    uint64_t test_data = (task_id << BITE_SIZE) | TEST_STOP_KO;       \
    uint64_t test_chan_handler;                                       \
    ax_channel_get(&test_chan_handler, "test_channel");               \
    ax_channel_snd(test_chan_handler, &test_data, sizeof(test_data)); \
  }

void test_end() {
  task_id_t task_id;
  ax_task_get(&task_id);
  uint64_t test_data = (task_id << BITE_SIZE) | TEST_STOP_OK;
  uint64_t test_chan_handler;
  ax_channel_get(&test_chan_handler, "test_channel");
  ax_channel_snd(test_chan_handler, &test_data, sizeof(test_data));
}

#define TEST_END() test_end()

#endif