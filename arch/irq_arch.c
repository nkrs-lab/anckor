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
#include "irq_arch.h"

#include "common.h"
#include "interrupt.h"
#include "panic.h"
#include "printk.h"
#include "registers.h"
#include "sched.h"
#include "task.h"

#define NB_INTERRUPT 3

// define a table to save all interrupt handlers
task_t *irq_table[NB_INTERRUPT] = {NULL};

/******************************************************************************
 * @brief enable the interrupt in IE register
 * @param interrupt identifier
 * @return None
 ******************************************************************************/
void interrupt_request(interrupt_id_t interrupt_id) {
  uint64_t reg_value;

  // translate kernel interrupt number to riscv arch interrupt register offset
  switch (interrupt_id) {
    case TIMER_INTERRUPT:
      reg_value = 1 << MIE_TIE_OFFSET;
      break;
    default:
      break;
  }

  // configure interrupt enable register
  csr_set(CSR_MIE, reg_value);

  // register the task in the dedicated interrupt handler
  irq_table[interrupt_id] = sched_get_current_task();
}

/******************************************************************************
 * @brief disable the interrupt in IE register
 * @param interrupt identifier
 * @return None
 ******************************************************************************/
void interrupt_release(interrupt_id_t interrupt_id) {
  uint64_t reg_value;

  // translate kernel interrupt number to riscv arch interrupt register offset
  switch (interrupt_id) {
    case TIMER_INTERRUPT:
      reg_value = 1 << MIE_TIE_OFFSET;
      break;
    default:
      break;
  }

  // reset interrupt enable register
  csr_clear(CSR_MIE, reg_value);

  // clear the interrupt handler pointer
  irq_table[interrupt_id] = NULL;
}

/******************************************************************************
 * @brief wake up the timer interrupt handler
 * @param none
 * @return none
 ******************************************************************************/
static inline void handle_timer_interrupt() {
  // disable timer interrupt
  csr_clear(CSR_MIE, MIE_TIE);

  if (irq_table[TIMER_INTERRUPT]) {
    // wake up registered interrupt handler
    // this will add the interrupt handler
    // in the run queue and run the scheduler
    // it will immediatly jump to the task if it
    // has the highest priority
    task_wakeup(irq_table[TIMER_INTERRUPT]);
    // re-enable global interrupts after having
    // woke up the driver task but before switching to it
    csr_set(CSR_MSTATUS, MACHINE_INTERRUPT_ENABLE);
    // switch to the interrupt handler task
    task_yield();
  } else {
    panic("Timer interrupt handler not defined\n");
  }
}

/******************************************************************************
 * @brief dispatch interrupt according to its source
 * @param none
 * @return none
 ******************************************************************************/
void dispatch_interrupt() {
  // get the interrupt cause from registers
  uint64_t cause = csr_read(CSR_MCAUSE) & CSR_MCAUSE_INTERRUPT_MASK;

  switch (cause) {
    case RISCV_INTERRUPT_MACHINE_SOFTWARE:
      panic("interrupt machine software n°%d not handled\n", cause);
      break;

    case RISCV_INTERRUPT_MACHINE_TIMER:
      handle_timer_interrupt();
      break;

    case RISCV_INTERRUPT_MACHINE_EXTERNAL:
      panic("interrupt machine external n°%d not handled\n", cause);
      break;

    default:
      panic("interrupt n°%d not handled\n", cause);
      break;
  }
}