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

#include "printk.h"
#include "processor.h"

#define STACK_SLOT_LIMIT 32  // there is 128k / 4K = 32 slots available

extern uint64_t _heap_start;

static uint64_t next_stack_slot = 0;

/**
 * @brief allocate memory for 4K stack to be used by tasks
 * @param none
 * @return stack_base_addr pointer
 */
uint64_t alloc_stack() {
  // check if we reach the limit of available stack slot
  if (next_stack_slot >= STACK_SLOT_LIMIT) return K_ERROR;

  // we have a slot available, so return its address
  uint64_t stack_base_addr = 0x800DA000 + next_stack_slot * STACK_SIZE;

  printk("stack base addr : 0x%x\r\n", stack_base_addr);

  // update the index of the next available slot
  next_stack_slot += 1;

  return stack_base_addr;
}