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
#include "channel.h"

#include "sched.h"
#include "stddef.h"
#include "string.h"
#include "task.h"

#define MAX_NB_CHANNEL          10
#define MAX_CHANNEL_NAME_LENGTH 20

typedef struct channel_t {
  char    name[MAX_CHANNEL_NAME_LENGTH];
  task_t *in;
  task_t *out;
  bool    rcv_rdy;
  bool    snd_rdy;
} channel_t;

channel_t channel[MAX_NB_CHANNEL];
uint64_t  channel_index = 0;

/******************************************************************************
 * channel snd / rcv procedures
 ******************************************************************************/
extern void _channel_snd(task_t *, task_t *, const uint64_t *);
extern void _channel_rcv(const uint64_t *);

/******************************************************************************
 * @brief get the channel struct from the channel handler
 * @param channel handler
 * @return none
 ******************************************************************************/
static inline channel_t *channel_get_from_handler(
    const uint64_t channel_handler) {
  return &channel[channel_handler];
}

/******************************************************************************
 * @brief create a communication channel between two tasks
 * @param channel handler
 * @param channel name
 * @return none
 ******************************************************************************/
k_return_t channel_create(uint64_t *channel_handler, const char *name) {
  if (channel_index >= MAX_NB_CHANNEL) {
    // we reached the maximum number of available channel
    return K_ERROR;
  }

  // register the channel name
  if (strlen(name) < MAX_CHANNEL_NAME_LENGTH) {
    strcpy(channel[channel_index].name, name);
  } else {
    return K_ERROR;
  }

  // create a channel
  channel[channel_index].in  = NULL;
  channel[channel_index].out = NULL;

  // initialize channel access flag
  channel[channel_index].rcv_rdy = false;
  channel[channel_index].snd_rdy = false;

  // return the channel ID to the calling thread
  *channel_handler = channel_index;

  // update next available channel index
  channel_index += 1;

  return K_OK;
};

/******************************************************************************
 * @brief return the channel handler corresponding to the given name if any
 * @param channel handler
 * @param channel name
 * @return none
 ******************************************************************************/
k_return_t channel_get(uint64_t *channel_handler, const char *name) {
  uint64_t chan_index = 0;
  while (chan_index <= MAX_NB_CHANNEL) {
    if (strcmp(channel[chan_index].name, name) == 0) {
      // we found the channel corresponding to the given name
      *channel_handler = chan_index;

      return K_OK;
    } else {
      chan_index += 1;
    }
  }

  return K_ERROR;
};

/******************************************************************************
 * @brief send a message through a communication channel
 * @param channel handler
 * @param msg pointer
 * @param length of message to send
 * @return none
 ******************************************************************************/
void channel_snd(const uint64_t channel_handler, const uint64_t *msg,
                 uint64_t msg_len) {
  // find channel from handler ID
  channel_t *channel = channel_get_from_handler(channel_handler);

  // register the sender task
  channel->in = sched_get_current_task();

  // set the task as ready to send
  channel->snd_rdy = true;

  // block until a rcv task is ready
  // we may be awaken up by another task
  while (!channel->rcv_rdy) {
    // there is no waiting task, go to BLOCKED state and
    // release the cpu
    task_set_state(channel->in, BLOCKED);
    // remove it from the run queue
    sched_remove_task(channel->in);

    sched_run();
  }

  // snd task is in RUNNING state, set it to READY state
  task_set_state(channel->in, READY);

  // there is a waiting task so switch to it
  sched_add_task(channel->out);
  task_set_state(channel->out, RUNNING);
  sched_set_current_task(channel->out);

  if (msg_len <= DOUBLE_WORD_SIZE) {
    // fast path for short messages
    // direct switch without calling the scheduler
    _channel_snd(channel->in, channel->out, msg);
  } else {
    // slow path
    // need to be implemented
  }

  // set the task as ready to send
  channel->snd_rdy = false;
};

/******************************************************************************
 * @brief receive a message through a communication channel
 * @param channel handler
 * @param msg pointer
 * @param length of message to send
 * @return none
 ******************************************************************************/
void channel_rcv(const uint64_t channel_handler, const uint64_t *msg,
                 uint64_t *msg_len) {
  // find channel from handler ID
  channel_t *channel = channel_get_from_handler(channel_handler);

  // register the rcv task
  channel->out = sched_get_current_task();

  // set the task as ready to receive
  channel->rcv_rdy = true;

  if (channel->snd_rdy) {
    // snd task is blocked, waiting to send its message
    task_set_state(channel->in, READY);
    // add it to the run queue
    sched_add_task(channel->in);
  }

  // release the cpu
  task_set_state(channel->out, BLOCKED);

  // remove it from the run queue
  sched_remove_task(channel->out);

  _channel_rcv(msg);

  // the message has been copied but not consumed. We can unblock
  // the snd thread but we uncheck the rcv_rdy flag to avoid the snd thread
  // to re-send a message.
  channel->rcv_rdy = false;

  // !!! unlock snd task here !!!
};