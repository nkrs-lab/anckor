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
#ifndef TASK_H
#define TASK_H

#include "common.h"
#include "processor.h"

typedef uint8_t stack_t[STACK_SIZE];

/******************************************************************************
 * @enum task_state_t
 * @brief used to store task current state
 ******************************************************************************/
typedef enum task_state_t {
  READY,
  RUNNING,
  BLOCKED,
} task_state_t;

/******************************************************************************
 * @struct task_id_t
 * @brief unique task id is composed of a VMS and thread ID
 ******************************************************************************/
typedef union task_id_t {
  uint32_t vms_id;
  uint32_t thread_id;
} task_id_t;

/******************************************************************************
 * @struct task_t
 * @brief structure to manage common thread and processes informations
 ******************************************************************************/
typedef struct task_t {
  const char  *name;
  task_id_t    task_id;
  uint8_t      prio;
  task_state_t state;
  stack_t     *stack;
  thread_t     thread;
} task_t;

/******************************************************************************
 * @brief initialize a task and schedule it
 * @param id of the task
 * @param function to run in the task
 * @param stack start address of the task
 * @param priority for the new task
 * @return none
 ******************************************************************************/
void task_create(const char *, void (*)(void), stack_t *, uint8_t);

/******************************************************************************
 * @brief task destroy
 *
 * This function deletes the task passed in argument from the system. It does
 * not call the scheduler
 *
 * @param task to destroy
 * @return none
 ******************************************************************************/
void task_destroy();

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
void task_runtime(void (*)(void));

/******************************************************************************
 * @brief initialize task stack
 *
 * @param stack start address pointer
 * @param size of the stack
 * @param function to run in the task context
 * @return top address of the initialized stack
 ******************************************************************************/
void task_stack_init(stack_t *, uint64_t, void (*)(void));

/******************************************************************************
 * @brief return the id of the current task
 * @param task_id pointer
 * @return none
 ******************************************************************************/
void task_get(task_id_t *);

/******************************************************************************
 * @brief yield the cpu to an another task
 * @param none
 * @return none
 ******************************************************************************/
void task_yield();

/******************************************************************************
 * @brief remove this thread from the run_queue but don't destroy its data
 * @param none
 * @return none
 ******************************************************************************/
void task_sleep();

/******************************************************************************
 * @brief wake up a task put on hold with task_sleep()
 * @param task_t address pointer
 * @return none
 ******************************************************************************/
void task_wakeup(task_t *);

/******************************************************************************
 * @brief task exit
 *
 * This function cleans all memory used to save task informations, this
 * comprises all stacks and associated structures. It also delete the task from
 * the scheduler runqueue.
 *
 * @param none
 * @return none
 ******************************************************************************/
void task_exit();

/******************************************************************************
 * @brief modify the state of the given task
 * @param task to modify
 * @return none
 ******************************************************************************/
inline void task_set_state(task_t *task, task_state_t state) {
  task->state = state;
}

#endif