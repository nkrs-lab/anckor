/*
 * Copyright (c) 2025 Qoda, engineering
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

#include "alloc.h"

#include "processor.h"

#define STACK_SLOT_LIMIT 32  // there is 128k / 4K = 32 slots available

extern uint64_t _heap_start;

static uint64_t next_stack_slot = 0;

/**
 * @brief allocate memory for 4K stack to be used by tasks
 * @param stack_base_addr pointer
 * @return -1 in case of error
 */
k_return_t alloc_stack(uint64_t *stack_base_addr) {
  // check if we reach the limit of available stack slot
  if (next_stack_slot >= STACK_SLOT_LIMIT) return K_ERROR;

  // we have a slot available, so return its address
  *stack_base_addr = (uint64_t)&_heap_start + next_stack_slot * STACK_SIZE;

  // update the index of the next available slot
  next_stack_slot += 1;

  return K_OK;
}