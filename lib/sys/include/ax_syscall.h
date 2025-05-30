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
#ifndef AX_SYSCALL_H
#define AX_SYSCALL_H

#include "interrupt.h"
#include "task.h"

extern k_return_t ax_task_create(const char *, void (*)(void), stack_t *,
                                 uint8_t);
extern void       ax_task_get(task_id_t *);
extern void       ax_task_destroy(task_t *);
extern void       ax_task_yield(void);
extern void       ax_task_sleep(void);
extern void       ax_task_wakeup(task_id_t);
extern void       ax_task_exit(void);
extern void       ax_interrupt_request(interrupt_id_t);
extern void       ax_interrupt_release(interrupt_id_t);
extern k_return_t ax_channel_create(uint64_t *, const char *);
extern k_return_t ax_channel_get(uint64_t *, const char *);
extern void       ax_channel_snd(const uint64_t, const uint64_t *, uint64_t);
extern void       ax_channel_rcv(const uint64_t, const uint64_t *, uint64_t *);

#endif