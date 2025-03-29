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
#ifndef PROCESSOR_H
#define PROCESSOR_H

#define STACK_SIZE 4096
#define DWORD_SIZE 8
#define LWORD_SIZE 16

/******************************************************************************
 * @struct thread_t
 * @brief structure used for thread local storage
 ******************************************************************************/
typedef struct thread_t {
  uint64_t sp;
  uint64_t gp;
} thread_t;

#endif