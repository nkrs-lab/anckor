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
#ifndef COMMON_H
#define COMMON_H

#include "stdbool.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define uint64_t unsigned long int
#define uint32_t unsigned int
#define uint16_t unsigned short int
#define uint8_t  unsigned char
#define bool_t   bool

#define DOUBLE_WORD_SIZE sizeof(uint64_t)
#define BITE_SIZE        8
#define BITE_MASK        0xFF

/******************************************************************************
 * @struct k_return_t
 * @brief general kernel error return code
 ******************************************************************************/
typedef enum k_return_t {
  K_ERROR = -1,
  K_OK    = 0,
} k_return_t;

/******************************************************************************
 * @brief write 64-bit registers
 * @param addr of the register
 * @param data to write
 * @return None
 ******************************************************************************/
inline void reg_write_double_word(const uint64_t addr, const uint64_t data) {
  volatile uint64_t *reg_addr = (uint64_t *)addr;
  *reg_addr                   = data;
}

/******************************************************************************
 * @brief read 64-bit registers
 * @param addr of the register
 * @return data in the register
 ******************************************************************************/
inline uint64_t reg_read_double_word(const uint64_t addr) {
  volatile uint64_t *reg_addr = (uint64_t *)addr;
  return (uint64_t)*reg_addr;
}

/******************************************************************************
 * @brief write 8-bit data in 64-bit registers
 * @param addr of the register
 * @param data to write
 * @param offset inthe 64-bit register
 * @return None
 ******************************************************************************/
inline void reg_write_byte(const uint64_t addr, const uint64_t offset,
                           const uint64_t data) {
  uint64_t val = reg_read_double_word(addr);
  val &= 0xFFFFFFFFFFFFFF00 << (8 * offset);
  val |= (uint64_t)data << (8 * offset);
  reg_write_double_word(addr, val);
}

#endif