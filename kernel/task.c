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
#include "task.h"

#include "ax_syscall.h"
#include "sched.h"
#include "stddef.h"

#define __no_return   __attribute__((noreturn))
#define VMS_ID_OFFSET 32

extern void _syscall(uint64_t syscall_number);

uint64_t last_thread_id = (uint64_t)NULL;

/******************************************************************************
 * @brief find a new thread_id
 * @param none
 * @return thread_id
 ******************************************************************************/
static uint64_t task_get_new_thread_id() {
  last_thread_id += 1;

  return last_thread_id;
}

/******************************************************************************
 * @brief task runtinme
 *
 * This function is used to run task_entry in a controlled environment. It cans
 * run an exit() procedure to properly clean the task if ever its main procedure
 * returns.
 *
 * @param function to run in the task
 * @return none
 ******************************************************************************/
__no_return void task_runtime(void (*task_entry)(void)) {
  // start the main task routine
  task_entry();

  // clean the task if ever it returns
  ax_task_exit();

  // tell the compiler we will never reach this point as the kernel should
  // at least run the init task
  __builtin_unreachable();
}

/******************************************************************************
 * @brief initialize a task and schedule it
 * @param id of the task
 * @param function to run in the task
 * @param stack start address of the task
 * @param priority for the new task
 * @return none
 ******************************************************************************/
void task_create(const char *name, void (*task_entry)(void), stack_t *stack,
                 uint8_t prio) {
  // save task infos at the beginning of the task
  task_t *task = (task_t *)stack;

  task->name = name;

  // find a unique task ID
  uint64_t vms_id    = 0;
  uint32_t thread_id = task_get_new_thread_id();
  task->task_id      = (vms_id << VMS_ID_OFFSET) | thread_id;

  // save task priority
  task->prio = prio;

  // all created tasks are placed in READY state
  task_set_state(task, READY);

  // save the stack base address
  task->stack = stack;

  // initialize task stack
  task_stack_init(stack, STACK_SIZE, task_entry);

  // save the new task in the run queue
  sched_add_task(task);
}

/******************************************************************************
 * @brief return the id of the current task
 * @param task_id pointer
 * @return none
 ******************************************************************************/
void task_get(task_id_t *task_id) {
  task_t *current_task = sched_get_current_task();

  *task_id = current_task->task_id;
}

/******************************************************************************
 * @brief yield the cpu to an another task
 * @param none
 * @return none
 ******************************************************************************/
void task_yield() {
  task_t *current_task = sched_get_current_task();

  task_set_state(current_task, READY);

  sched_run();
}

/******************************************************************************
 * @brief remove this thread from the run_queue but don't destroy its data
 * @param none
 * @return none
 ******************************************************************************/
void task_sleep() {
  // get the current_task task
  task_t *current_task = sched_get_current_task();

  task_set_state(current_task, BLOCKED);
  // remove it from the run queue
  sched_remove_task(current_task);
  // call the scheduler
  sched_run();
}

/******************************************************************************
 * @brief wake up a task put on hold with task_sleep()
 * @param task id
 * @return none
 ******************************************************************************/
void task_wakeup(task_id_t task_id) {
  // find task from its ID
  task_t *task = task_find(task_id);
  // move the state from BLOCKED to READY
  task_set_state(task, READY);
  // add the task to the run queue
  sched_add_task(task);
}

/******************************************************************************
 * @brief task exit
 *
 * This function cleans all memory used to save task information, this
 * comprises all stacks and associated structures. It also deletes the task from
 * the scheduler runqueue.
 *
 * It is used when task_entry returns to clean up thread's memory
 *
 * @param none
 * @return none
 ******************************************************************************/
void task_exit() {
  task_t *task = sched_get_current_task();
  // tag the deleted task as blocked
  task_set_state(task, BLOCKED);
  // remove it from the run queue
  sched_remove_task(task);
  // call the scheduler
  sched_run();
}

/******************************************************************************
 * @brief task delete
 *
 * This function cleans all memory used to save task information, this
 * comprises all stacks and associated structures. It also deletes the task from
 * the scheduler runqueue.
 *
 * Instead of task_exit, it's used to delete a task from an another task
 *
 * @param task to delete
 * @return none
 ******************************************************************************/
void task_destroy(task_t *task) {
  // tag the deleted task as blocked
  task_set_state(task, BLOCKED);
  // remove it from the run queue
  sched_remove_task(task);
}