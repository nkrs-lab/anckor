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

#include "sched.h"

#include "processor.h"
#include "stddef.h"

#define MAX_PRIO  255
#define IDLE_PRIO 0

#define __idle_task_data __attribute__((section(".data.idle_task")))

/******************************************************************************
 * context switch procedure
 ******************************************************************************/
extern void _switch_to(thread_t *prev_thread, thread_t *next_thread);

/******************************************************************************
 * idle stack / task are statically defined
 ******************************************************************************/
extern stack_t   idle_stack;
task_t idle_task __idle_task_data = {
    .task_id = 0,
    .prio    = IDLE_PRIO,
    .state   = READY,
    .stack   = &idle_stack,
};

/******************************************************************************
 * main run_queue used by the scheduler
 ******************************************************************************/
static task_t *run_queue[MAX_PRIO];
static task_t *current_task;

/******************************************************************************
 * @brief find the next task to run
 * @param none
 * @return task to run
 ******************************************************************************/
task_t *sched_get_next_task() {
  // look over the run queue for a task to schedule
  uint8_t prio_idx = MAX_PRIO - 1;
  while (prio_idx >= 0) {
    if (run_queue[prio_idx]) {
      // there is a task to run
      break;
    }

    prio_idx -= 1;
  }

  return run_queue[prio_idx];
}

/******************************************************************************
 * @brief context switch from the scheduler
 *
 * This function is used to switch between two task contexts. It should not be
 * modified without careful attention: the task stack design heavily relies on
 * how this function saves parameters on the stack.
 *
 * The main use of sched_switch is saving RA before jumping to _switch_to and
 * restore the same register at the output
 *
 * @param previous_task address pointer
 * @param next_task address pointer
 * @return none
 ******************************************************************************/
void sched_switch(task_t *prev_task, task_t *new_task) {
  _switch_to(&prev_task->thread, &new_task->thread);
}

/******************************************************************************
 * @brief main function to run the scheduler
 * @param none
 * @return none
 ******************************************************************************/
void sched_run() {
  task_t *new_task;
  task_t *prev_task;

  // save the current task
  prev_task = sched_get_current_task();

  // get the new task to run
  new_task = sched_get_next_task();
  task_set_state(new_task, RUNNING);

  // update the current task
  sched_set_current_task(new_task);

  sched_switch(prev_task, new_task);
}

/******************************************************************************
 * @brief add a new task to the run queue
 * @param task to add in the run queue
 * @return none
 ******************************************************************************/
void sched_add_task(task_t *task) {
  run_queue[task->prio] = task;
}

/******************************************************************************
 * @brief remove a task from the run queue
 * @param task to remove from the run queue
 * @return none
 ******************************************************************************/
void sched_remove_task(task_t *task) {
  run_queue[task->prio] = NULL;
}

/******************************************************************************
 * @brief find the current running task
 * @param none
 * @return current_task address pointer
 ******************************************************************************/
task_t *sched_get_current_task() {
  return current_task;
}

/******************************************************************************
 * @brief set the current running task
 * @param current_task address pointer
 * @return none
 ******************************************************************************/
void sched_set_current_task(task_t *task) {
  current_task = task;
}

/******************************************************************************
 * @brief initiliaze scheduler parameters
 * @param none
 * @return none
 ******************************************************************************/
void sched_init() {
  current_task = &idle_task;

  sched_add_task(&idle_task);
}