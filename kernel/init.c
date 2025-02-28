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

#include "init.h"

#include "alloc.h"
#include "app.h"
#include "ax_syscall.h"
#include "banner.h"
#include "part_table.h"
#include "task.h"

#define INIT_PRIO 1

extern uint64_t _part_table_start;

/******************************************************************************
 * @brief Init task will launch all registered tasks in the system
 * @param None
 * @return None
 ******************************************************************************/
void init_run(void) {
  stack_t *app_stack = NULL;

  // read partition table informations from the configuration area
  partition_info_t *partition_table = (partition_info_t *)&_part_table_start;

  // allocate memory for the stack
  alloc_stack((uint64_t *)&app_stack);

  // create a task for the partition
  ax_task_create(partition_table[0].name, (void *)partition_table[0].entry,
                 app_stack, partition_table[0].prio);

  // display kernel banner at the end of the init stage
  banner_display();
}

/******************************************************************************
 * @brief create the init task
 * @param None
 * @return None
 ******************************************************************************/
void init_create(void) {
  stack_t *init_stack = NULL;

  // allocate memory for the stack
  alloc_stack((uint64_t *)&init_stack);

  ax_task_create("init_task", init_run, init_stack, INIT_PRIO);
}