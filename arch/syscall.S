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

#include "registers.h"
#include "offsets.h"
#include "syscall.h"

 /*
 * ax_task_create syscall
 *
 * a0: task name
 * a1: task entry
 * a2: task stack
 * a3: task priority
 * a4: not used
 * a5: not used
 * a6: not used
 * a7: syscall number
 *
 */

# use .global keyworkd makes the symbol visible to the linker
.section .syscall_wrappers
.global ax_task_create
ax_task_create:
    li a7, SYSCALL_TASK_CREATE
    # rise exception, this will update mepc register but left
    # the ra register unchanged
    ecall
    ret

 /*
 * ax_task_destroy syscall
 *
 * a0: task to delete
 * a1: not used
 * a2: not used
 * a3: not used
 * a4: not used
 * a5: not used
 * a6: not used
 * a7: syscall number
 *
 */

# use .global keyworkd makes the symbol visible to the linker
.section .syscall_wrappers
.global ax_task_destroy
ax_task_destroy:
    li a7, SYSCALL_TASK_DESTROY
    # rise exception, this will update mepc register but left
    # the ra register unchanged
    ecall
    ret


 /*
 * ax_task_yield syscall
 *
 * a0: not used
 * a1: not used
 * a2: not used
 * a3: not used
 * a4: not used
 * a5: not used
 * a6: not used
 * a7: syscall number
 *
 */
.section .syscall_wrappers
.global ax_task_yield
ax_task_yield:
    li a7, SYSCALL_TASK_YIELD
    # rise exception, this will update mepc register but left
    # the ra register unchanged
    ecall
    ret

 /*
 * ax_task_sleep syscall
 *
 * a0: not used
 * a1: not used
 * a2: not used
 * a3: not used
 * a4: not used
 * a5: not used
 * a6: not used
 * a7: syscall number
 *
 */
.section .syscall_wrappers
.global ax_task_sleep
ax_task_sleep:
    li a7, SYSCALL_TASK_SLEEP
    # rise exception, this will update mepc register but left
    # the ra register unchanged
    ecall
    ret

 /*
 * ax_task_wakeup syscall
 *
 * a0: task_t *task
 * a1: not used
 * a2: not used
 * a3: not used
 * a4: not used
 * a5: not used
 * a6: not used
 * a7: syscall number
 *
 */
.section .syscall_wrappers
.global ax_task_wakeup
ax_task_wakeup:
    li a7, SYSCALL_TASK_WAKEUP
    # rise exception, this will update mepc register but left
    # the ra register unchanged
    ecall
    ret

 /*
 * ax_task_exit syscall
 *
 * a0: not used
 * a1: not used
 * a2: not used
 * a3: not used
 * a4: not used
 * a5: not used
 * a6: not used
 * a7: syscall number
 *
 */
.section .syscall_wrappers
.global ax_task_exit
ax_task_exit:
    li a7, SYSCALL_TASK_EXIT
    # rise exception, this will update mepc register but left
    # the ra register unchanged
    ecall
    ret

 /*
 * ax_interrupt_request syscall
 *
 * a0: interrupt id
 * a1: not used
 * a2: not used
 * a3: not used
 * a4: not used
 * a5: not used
 * a6: not used
 * a7: syscall number
 *
 */
.section .syscall_wrappers
.global ax_interrupt_request
ax_interrupt_request:
    li a7, SYSCALL_INTERRUPT_REQUEST
    # rise exception, this will update mepc register but left
    # the ra register unchanged
    ecall
    ret

 /*
 * ax_interrupt_release syscall
 *
 * a0: interrupt id
 * a1: not used
 * a2: not used
 * a3: not used
 * a4: not used
 * a5: not used
 * a6: not used
 * a7: syscall number
 *
 */
.section .syscall_wrappers
.global ax_interrupt_release
ax_interrupt_release:
    li a7, SYSCALL_INTERRUPT_RELEASE
    # rise exception, this will update mepc register but left
    # the ra register unchanged
    ecall
    ret

 /*
 * ax_channel_create syscall
 *
 * a0: channel handler pointer
 * a1: channel name pointer
 * a2: not used
 * a3: not used
 * a4: not used
 * a5: not used
 * a6: not used
 * a7: syscall number
 *
 */
.section .syscall_wrappers
.global ax_channel_create
ax_channel_create:
    li a7, SYSCALL_CHANNEL_CREATE
    # rise exception, this will update mepc register but left
    # the ra register unchanged
    ecall
    ret

 /*
 * ax_channel_get syscall
 *
 * a0: channel handler pointer
 * a1: channel name pointer
 * a2: not used
 * a3: not used
 * a4: not used
 * a5: not used
 * a6: not used
 * a7: syscall number
 *
 */
.section .syscall_wrappers
.global ax_channel_get
ax_channel_get:
    li a7, SYSCALL_CHANNEL_GET
    # rise exception, this will update mepc register but left
    # the ra register unchanged
    ecall
    ret

 /*
 * ax_channel_snd syscall
 *
 * a0: channel handler
 * a1: data pointer
 * a2: data length
 * a3: not used
 * a4: not used
 * a5: not used
 * a6: not used
 * a7: syscall number
 *
 */
.section .syscall_wrappers
.global ax_channel_snd
ax_channel_snd:
    li a7, SYSCALL_CHANNEL_SND
    # rise exception, this will update mepc register but left
    # the ra register unchanged
    ecall
    ret

 /*
 * ax_channel_rcv syscall
 *
 * a0: channel handler
 * a1: data pointer
 * a2: data length
 * a3: not used
 * a4: not used
 * a5: not used
 * a6: not used
 * a7: syscall number
 *
 */
.section .syscall_wrappers
.global ax_channel_rcv
ax_channel_rcv:
    li a7, SYSCALL_CHANNEL_RCV
    # rise exception, this will update mepc register but left
    # the ra register unchanged
    ecall
    ret

 /*
 * table to save all syscall handlers
 *
 */
.section .rodata
.align RISCV_PTR_LENGTH
.global _syscall_table
.global _syscall_table_end
_syscall_table:
    .dword task_create
    .dword task_destroy
    .dword task_yield
    .dword task_sleep
    .dword task_wakeup
    .dword task_exit
    .dword interrupt_request
    .dword interrupt_release
    .dword channel_create
    .dword channel_get
    .dword channel_snd
    .dword channel_rcv
    .dword sys_default
    .dword sys_default
    .dword sys_default
    .dword sys_default
    .dword sys_default
    .dword sys_default
    .dword sys_default
    .dword sys_default
    .dword sys_default
    .dword sys_default
    .dword sys_default
    .dword sys_default
    .dword sys_default
    .dword sys_default
    .dword sys_default
    .dword sys_default
    .dword sys_default
    .dword sys_default
    .dword sys_default
    .dword sys_default
_syscall_table_end: