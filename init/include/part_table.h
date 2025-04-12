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

typedef struct {
  const char name[PARTITION_NAME_LENGTH];
  uint8_t    prio;
  void (*entry)(void);
} partition_info_t;

#endif