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

#ifndef ALLOC_H
#define ALLOC_H

#include "common.h"

/**
 * @brief allocate memory for 4K stack to be used by tasks
 * @param stack_base_addr pointer
 * @return -1 in case of error
 */
k_return_t alloc_stack(uint64_t *);

#endif