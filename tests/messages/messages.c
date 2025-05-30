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

#include "ax_syscall.h"
#include "interrupt.h"
#include "irq_arch.h"
#include "printf.h"
#include "test.h"

#define MAGIC_WORD   0x5DA9CAB3
#define MAGIC_WORD_2 0x5DA9DA57

/*******************************************************************************
 * Definitions
 ******************************************************************************/
stack_t snd_messages_thread_stack;
stack_t rcv_messages_thread_stack;
stack_t snd_messages_thread_stack_2;
stack_t rcv_messages_thread_stack_2;
stack_t messages_thread_stack;

/******************************************************************************
 * @brief initalize message and send it
 * @param None
 * @return None
 ******************************************************************************/
void snd_messages_thread(void) {
  uint64_t data_to_send = MAGIC_WORD;
  uint64_t snd_chan_handler;

  // looking for the channel
  if (ax_channel_get(&snd_chan_handler, "data_channel") < 0) {
    // we didn't find the channel
    TEST_ASSERT(false);
  } else {
    // send data through the channel
    ax_channel_snd(snd_chan_handler, &data_to_send, sizeof(data_to_send));
  }
}

/******************************************************************************
 * @brief receive message
 * @param None
 * @return None
 ******************************************************************************/
void rcv_messages_thread(void) {
  uint64_t data_to_receive = 0;
  uint64_t data_len        = 0;
  uint64_t rcv_chan_handler;

  // create a communication channel
  ax_channel_create(&rcv_chan_handler, "data_channel");

  // receive the message
  ax_channel_rcv(rcv_chan_handler, &data_to_receive, &data_len);

  TEST_ASSERT(data_to_receive == MAGIC_WORD);
}

/******************************************************************************
 * @brief initalize message and send it
 * @param None
 * @return None
 ******************************************************************************/
void snd_messages_thread_2(void) {
  uint64_t data_to_send = MAGIC_WORD_2;
  uint64_t snd_chan_handler;

  // create a communication channel
  ax_channel_create(&snd_chan_handler, "data_channel_2");

  // send data through the channel
  ax_channel_snd(snd_chan_handler, &data_to_send, sizeof(data_to_send));
}

/******************************************************************************
 * @brief receive message
 * @param None
 * @return None
 ******************************************************************************/
void rcv_messages_thread_2(void) {
  uint64_t data_to_receive = 0;
  uint64_t data_len        = 0;
  uint64_t rcv_chan_handler;

  // looking for the channel
  if (ax_channel_get(&rcv_chan_handler, "data_channel_2") < 0) {
    // we didn't find the channel
    TEST_ASSERT(false);
  } else {
    // receive the message
    ax_channel_rcv(rcv_chan_handler, &data_to_receive, &data_len);
  }

  TEST_ASSERT(data_to_receive == MAGIC_WORD_2);
}

/******************************************************************************
 * @brief create the channel and the send / rcv threads
 * @param None
 * @return None
 ******************************************************************************/
void main(void) {
  TEST_BEGIN();
  // CASE 1

  // create the send thread
  ax_task_create("snd_messages_test", snd_messages_thread,
                 &snd_messages_thread_stack, 11);

  // create the rcv thread
  ax_task_create("rcv_messages_test", rcv_messages_thread,
                 &rcv_messages_thread_stack, 12);

  ax_task_yield();

  // CASE 2

  // create the send thread
  ax_task_create("snd_messages_test_2", snd_messages_thread_2,
                 &snd_messages_thread_stack_2, 12);

  // create the rcv thread
  ax_task_create("rcv_messages_test_2", rcv_messages_thread_2,
                 &rcv_messages_thread_stack_2, 11);

  ax_task_yield();

  // end of test, return to ATE engine
  TEST_END();
}