/* Copyright (c) 2023 Qoda, engineering

This program is free software; you can redistribute it and/or modify 
it under the terms and conditions of the GNU General Public License,
version 3 or later, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received copies of the GNU General Public License and
the GNU Lesser General Public License along with this program.  If
not, see https://www.gnu.org/licenses/ */

/*
Include predefined mappings for syscalls
*/
INCLUDE tools/linker/syscalls.ld

/*
riscv is the name of the architecture that the linker understands
for any RISC-V target (64-bit or 32-bit).
*/
OUTPUT_ARCH( "riscv" )

ENTRY(_partition_runtime)

/*
Each partition uses 32kbyte of memory. 
It starts at offset 0x100000 as 1MByte is reserved for the kernel.
*/
MEMORY
{
  ram (wxa) : ORIGIN = 0x80100000, LENGTH = 32K
}

/*
we declare 3 sections in the .ELF program headers and specify 
these sections as loadable
*/
PHDRS
{
  text PT_LOAD FLAGS(5);   /* PF_R|PF_X */
  rodata PT_LOAD FLAGS(4); /* PF_R */
  data PT_LOAD FLAGS(6);   /* PF_R|PF_W */
  bss PT_LOAD FLAGS(6);   /* PF_R|PF_W */
}

SECTIONS
{
  .text : {
    PROVIDE(_text_start = .);
    *(.text.init) 
	  *(.text .text.*)
    *(.gnu.linkonce.t.*)
    PROVIDE(_text_end = .);
  } >ram AT>ram :text

  PROVIDE(_global_pointer_part = .);

  /* 
  global read only data
  */
  .rodata : {
    PROVIDE(_rodata_start = .);
    *(.rodata .rodata.*)
    *(.gnu.linkonce.r.*)
    PROVIDE(_rodata_end = .);
  } >ram AT>ram :rodata

  . = ALIGN(4K);

  /* 
  global initialized data 
  */
  .data : {
    PROVIDE(_data_start = .);
    *(.sdata .sdata.*)
    *(.gnu.linkonce.s.*)
	  *(.data .data.*)
    *(.gnu.linkonce.d.*)
    PROVIDE(_data_end = .);
  } >ram AT>ram :data

  /* 
  global uninitialized data 
  */
  .bss : {
    PROVIDE(_bss_start_part = .);
    *(.sbss .sbss.*) 
    *(.gnu.linkonce.sb.*)
	  *(.bss .bss.*)
    *(.gnu.linkonce.b.*)
    PROVIDE(_bss_end_part = .);
  } >ram AT>ram :bss
}