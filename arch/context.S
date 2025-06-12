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

 /*
 * Switch from a previous thread context to the next thread context
 * Only callee saved registers are stored and loaded.
 *
 * a0: thread_t to store
 * a1: thread_t to load
 *
 */
#include "offsets.h"

# use .global keyworkd makes the symbol visible to the linker
.global _switch_to
_switch_to:
    # move down sp to have space to save callee saved registers
    add	sp, sp, -CALLEE_STACK_FRAME_LENGTH
    # save previous thread context
    sd s0, CALLEE_STACK_FRAME_S0(sp)
    sd s1, CALLEE_STACK_FRAME_S1(sp)
    sd s2, CALLEE_STACK_FRAME_S2(sp)
    sd s3, CALLEE_STACK_FRAME_S3(sp)
    sd s4, CALLEE_STACK_FRAME_S4(sp)
    sd s5, CALLEE_STACK_FRAME_S5(sp)
    sd s6, CALLEE_STACK_FRAME_S6(sp)
    sd s7, CALLEE_STACK_FRAME_S7(sp)
    sd s8, CALLEE_STACK_FRAME_S8(sp)
    sd s9, CALLEE_STACK_FRAME_S9(sp)
    sd s10, CALLEE_STACK_FRAME_S10(sp)
    sd s11, CALLEE_STACK_FRAME_S11(sp)
    # save global pointer in thread structure
    sd gp, TASK_THREAD_GP(a0)
    # save stack pointer in thread structure
    sd sp, TASK_THREAD_SP(a0)

    # load stack pointer from new thread context
    ld sp, TASK_THREAD_SP(a1)
    # load global pointer from new thread context
    ld gp, TASK_THREAD_GP(a0)
    # load callee saved regs from next thread context
    ld s0, CALLEE_STACK_FRAME_S0(sp)
    ld s1, CALLEE_STACK_FRAME_S1(sp)
    ld s2, CALLEE_STACK_FRAME_S2(sp)
    ld s3, CALLEE_STACK_FRAME_S3(sp)
    ld s4, CALLEE_STACK_FRAME_S4(sp)
    ld s5, CALLEE_STACK_FRAME_S5(sp)
    ld s6, CALLEE_STACK_FRAME_S6(sp)
    ld s7, CALLEE_STACK_FRAME_S7(sp)
    ld s8, CALLEE_STACK_FRAME_S8(sp)
    ld s9, CALLEE_STACK_FRAME_S9(sp)
    ld s10, CALLEE_STACK_FRAME_S10(sp)
    ld s11, CALLEE_STACK_FRAME_S11(sp)
    # move up sp to have space to save callee saved registers
    add	sp, sp, CALLEE_STACK_FRAME_LENGTH
    
    ret