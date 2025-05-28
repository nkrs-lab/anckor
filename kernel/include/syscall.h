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
#ifndef SYSCALL_H
#define SYSCALL_H

#define SYSCALL_MAX_NB 32

#define SYSCALL_TASK_CREATE       0
#define SYSCALL_TASK_GET          1
#define SYSCALL_TASK_DESTROY      2
#define SYSCALL_TASK_YIELD        3
#define SYSCALL_TASK_SLEEP        4
#define SYSCALL_TASK_WAKEUP       5
#define SYSCALL_TASK_EXIT         6
#define SYSCALL_INTERRUPT_REQUEST 7
#define SYSCALL_INTERRUPT_RELEASE 8
#define SYSCALL_CHANNEL_CREATE    9
#define SYSCALL_CHANNEL_GET       10
#define SYSCALL_CHANNEL_SND       11
#define SYSCALL_CHANNEL_RCV       12

#endif