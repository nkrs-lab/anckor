/*
 * Copyright (c) 2025 Qoda, engineering
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

#include "./include/part_table.h"

partition_info_t partition_table[] = {
    {.name  = "ATE partition",
     .prio  = 10,
     .entry = (void *)PARTITION_0_BASE_ADDR},
    {.name  = "Test App partition",
     .prio  = 5,
     .entry = (void *)PARTITION_1_BASE_ADDR},
    {.name  = "Test Threads partition",
     .prio  = 4,
     .entry = (void *)PARTITION_2_BASE_ADDR},
    {.name  = "Test Interrupt partition",
     .prio  = 2,
     .entry = (void *)PARTITION_3_BASE_ADDR},
    {.name = "MAGIC_WORD", .prio = 0, .entry = (void *)MAGIC_WORD}};