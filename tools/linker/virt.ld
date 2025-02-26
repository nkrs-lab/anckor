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
riscv is the name of the architecture that the linker understands
for any RISC-V target (64-bit or 32-bit).
*/
OUTPUT_ARCH( "riscv" )

/*
we tell the link which procedure is the program entry point
*/
ENTRY( _start )

/*
we declare one memory region of 8MBytes to store our program.
This sections starts at address 0x8000_0000 as specified in 
Qemu for riscv virt machine.
*/
MEMORY
{
  ram (wxa) : ORIGIN = 0x80000000, LENGTH = 896k
  heap (wxa) : ORIGIN = 0x800DA000, LENGTH = 128k
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
  stack PT_LOAD FLAGS(6);
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

  PROVIDE(_global_pointer = .);

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

    /* 
    idle stack section
    */
    . = ALIGN(4K);
    _idle_stack_start = .;
    PROVIDE(idle_stack = .);
    KEEP(*(.data.idle_task));
    . = _idle_stack_start + 4K;
    _idle_stack_end = .;

    /* 
    Apps section
    */
    PROVIDE(_apps_start = .);
    KEEP(*(.data.apps));
    PROVIDE(_apps_end = .);

    /* 
    tests section
    */
    PROVIDE(_tests_start = .);
    KEEP(*(.data.tests));
    PROVIDE(_tests_end = .);

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
    PROVIDE(_bss_start = .);
    *(.sbss .sbss.*) 
    *(.gnu.linkonce.sb.*)
	  *(.bss .bss.*)
    *(.gnu.linkonce.b.*)
    PROVIDE(_bss_end = .);
  } >ram AT>ram :bss

  /* 
  A dedicated section to store stack data
  */
  .stack : {
    PROVIDE(_heap_start = .);
	  KEEP(*(.stack .stack.*))
    . = _heap_start + 128k;
    PROVIDE(_heap_end = .);
  } >heap AT>heap :stack
}