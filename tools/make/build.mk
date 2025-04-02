# Copyright (c) 2023 Qoda, engineering

# This program is free software; you can redistribute it and/or modify 
# it under the terms and conditions of the GNU General Public License,
# version 3 or later, as published by the Free Software Foundation.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.

# You should have received copies of the GNU General Public License and
# the GNU Lesser General Public License along with this program.  If
# not, see https://www.gnu.org/licenses/

# TOP LEVEL MAKEFILE

include tools/make/macros.mk

include tools/generated/config.mk

OBJCPY := riscv64-unknown-elf-objcopy
LD := riscv64-unknown-elf-ld
# LINKER_SCRIPT is defined in python and passed to the makefile as an argument
LINKER_SCRIPT ?= default.ld
GLOBAL_LDFLAGS := -nostdlib -Map build/output.map $(LINKER_SCRIPT)

DEBUG_FLAG ?= false
ifeq ($(DEBUG_FLAG), true)
	GLOBAL_LDFLAGS += -g
endif

# COMPILE_LIST is defined in python and passed to the makefile as an argument
GLOBAL_LIST := $(COMPILE_LIST)

.PHONY: all build run

all: clean build

clean: 
# delete build directory if it already exists
	@if [ -d "build" ]; then \
		rm -r build; \
	fi

setup_build_dir: clean
	@mkdir build

MODULE_TARGET_LIST :=

MODULE_DEPS :=
MODULE_INCS :=

MODULE_CSRCS :=
MODULE_CINCS :=

MODULE_ASMSRCS :=
MODULE_ASMINCS :=

MODULE_COBJS :=
MODULE_ASMOBJS := 

MODULE_CTARGETS := 
MODULE_ASMTARGETS := 

MODULE_CFLAGS := 

GLOBAL_OBJECTS_LIST :=

include tools/make/collect.mk

# MODULE_TARGET_LIST contains all modules to build before linking
build: $(MODULE_TARGET_LIST)
# link all components
	$(info link all objects files)
	@$(LD) $(GLOBAL_LDFLAGS) $(GLOBAL_OBJECTS_LIST) -o build/$(BUILD_TARGET)

generate_kernel_img:
	$(info generate kernel image)
# generate binary files for kernel core and modules from elf files
	@$(OBJCPY) -O binary build/core.elf build/core.img
	@$(OBJCPY) -O binary build/part_table.elf build/part_table.img
	@$(OBJCPY) -O binary build/part.elf build/part.img
# merge all binary files in a single executable
	@truncate -s 8M build/anckor.img
	@dd if=build/core.img of=build/anckor.img bs=1 seek=0 conv=notrunc
	@dd if=build/part_table.img of=build/anckor.img bs=1 seek=1020k conv=notrunc
	@dd if=build/part.img of=build/anckor.img bs=1 seek=1M conv=notrunc