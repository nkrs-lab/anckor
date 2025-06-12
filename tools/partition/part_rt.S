/*
 * Copyright (c) 2025 Assembly Lab
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

.option norvc

# place this routine at the top of the binary file, this is the
# program entry point
.section .text.init
# use .global keyworkd makes the symbol visible to the linker
.global _partition_runtime
_partition_runtime:
	add	    sp, sp, -16
    sd      ra, 0(sp)
.option push
.option norelax
	la		gp, _global_pointer_part
.option pop
	# initialize bss section to zero
	la 		a0, _bss_start_part
	la		a1, _bss_end_part
	bgeu	a0, a1, 2f
1:
	sd		zero, (a0)
	addi	a0, a0, 8
	bltu	a0, a1, 1b
2:
	# jump to the partition main routine
    call    main
	# we return to task_rt routine
	ld	    ra, 0(sp)
    add	    sp, sp, 16
	ret