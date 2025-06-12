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

#ifndef PART_TABLE_H
#define PART_TABLE_H

#include "common.h"

#define PARTITION_NAME_LENGTH 50

#define MAGIC_WORD 0x55AA5A5A

// partitions memory section starts at 1MB offset
#define PARTITION_BASE_ADDR 0x80100000
// each partition has a 32KB memory size
#define PARTITION_SIZE        0x8000
#define PARTITION_0_BASE_ADDR PARTITION_BASE_ADDR
#define PARTITION_1_BASE_ADDR PARTITION_BASE_ADDR + PARTITION_SIZE
#define PARTITION_2_BASE_ADDR PARTITION_BASE_ADDR + 2 * PARTITION_SIZE
#define PARTITION_3_BASE_ADDR PARTITION_BASE_ADDR + 3 * PARTITION_SIZE
#define PARTITION_4_BASE_ADDR PARTITION_BASE_ADDR + 4 * PARTITION_SIZE

typedef struct {
  const char name[PARTITION_NAME_LENGTH];
  uint8_t    prio;
  void (*entry)(void);
} partition_info_t;

#endif