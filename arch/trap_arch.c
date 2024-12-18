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
#include "common.h"
#include "panic.h"
#include "printk.h"
#include "registers.h"

/******************************************************************************
 * @brief handle unknown exceptions
 * @return none
 ******************************************************************************/
void handle_unknown_exception() {
  uint64_t cause = csr_read(CSR_MCAUSE) & CSR_MCAUSE_INTERRUPT_MASK;
  // get the cause of the exception
  printk("exception not handled / mcause : %d\r\n", cause);

  hang_processor();
}

/******************************************************************************
 * @brief handle unused syscalls
 * @param syscall number
 * @return none
 ******************************************************************************/
void sys_default(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4,
                 uint64_t arg5, uint64_t arg6, uint64_t arg7,
                 uint64_t syscall_number) {
  printk("syscall nb° %d is not implemented.\n ", syscall_number);

  hang_processor();
}
