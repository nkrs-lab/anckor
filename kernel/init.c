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
#include "task.h"

#define INIT_PRIO 1

extern uint64_t _apps_start;
extern uint64_t _apps_end;

/******************************************************************************
 * @brief Init task will launch all registered tasks in the system
 * @param None
 * @return None
 ******************************************************************************/
void init_run(void) {
  // iterate over all app descriptors saved in the section(.data.apps)
  for (uint64_t *app_pt = &_apps_start; app_pt < &_apps_end; app_pt += 1) {
    // get the app descriptor from the current pointer
    app_info_t *app = (app_info_t *)*app_pt;
    // create a task for the app
    ax_task_create(app->name, app->entry, app->stack, app->prio);
  }

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
  init_stack = (stack_t *)alloc_stack();

  ax_task_create("init_task", init_run, init_stack, INIT_PRIO);
}