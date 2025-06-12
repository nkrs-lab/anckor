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

BUILD_DIR := build

CC := riscv64-unknown-elf-gcc
GLOBAL_CFLAGS := -Wall -march=rv64gc -mabi=lp64 -fpie -ffreestanding

ifeq ($(DEBUG_FLAG), true)
	MODULE_CFLAGS += -Og -ggdb
else 
	MODULE_CFLAGS += -O1
endif

# manage dependencies
MODULE_DEPS_INCS := $(addsuffix /include,$(addprefix -I, $(MODULE_DEPS)))
MODULE_INCS := $(MODULE_DEPS_INCS)
MODULE_INCS += -I$(MODULE_ID)/include

# add sources for the current module
MODULE_CSRCS := $(wildcard $(MODULE_ID)/*.c)
MODULE_ASMSRCS := $(wildcard $(MODULE_ID)/*.S)

MODULE_CINCS := $(MODULE_INCS)
MODULE_ASMINCS := $(MODULE_INCS)

MODULE_COBJS := $(MODULE_CSRCS:.c=.o)
MODULE_ASMOBJS := $(MODULE_ASMSRCS:.S=.o)

MODULE_CTARGETS := $(addprefix $(BUILD_DIR)/, $(MODULE_COBJS))
MODULE_ASMTARGETS := $(addprefix $(BUILD_DIR)/, $(MODULE_ASMOBJS))

# update global module list
MODULE_TARGET_LIST += $(MODULE_ID)
GLOBAL_OBJECTS_LIST += $(MODULE_CTARGETS)
GLOBAL_OBJECTS_LIST += $(MODULE_ASMTARGETS)

# use target specific variables to set module specific variables
$(MODULE_ID): MODULE_CINCS := $(MODULE_CINCS)
$(MODULE_ID): MODULE_ASMINCS := $(MODULE_ASMINCS)
$(MODULE_ID): MODULE_CFLAGS := $(MODULE_CFLAGS)

# declare C and ASM targets
.PHONY: build

$(BUILD_DIR)/$(MODULE_ID)/%.o: $(MODULE_ID)/%.c
	@$(MKDIR)
	$(info compiling $<)
	@$(CC) $(GLOBAL_CFLAGS) $(MODULE_CFLAGS) $(MODULE_CINCS) -c $< -o $@

$(BUILD_DIR)/$(MODULE_ID)/%.o: $(MODULE_ID)/%.S
	@$(MKDIR)
	$(info compiling $<)
	@$(CC) $(GLOBAL_CFLAGS) $(MODULE_CFLAGS) $(MODULE_ASMINCS) -c $< -o $@

$(MODULE_ID): $(MODULE_CTARGETS) $(MODULE_ASMTARGETS)

# reset variables set here
MODULE_DEPS :=
MODULE_DEPS_INCS :=
MODULE_INCS :=
MODULE_CSRCS :=
MODULE_ASMSRCS :=
MODULE_CINCS :=
MODULE_ASMINCS :=
MODULE_COBJS :=
MODULE_ASMOBJS :=
MODULE_CFLAGS := 
MODULE_CTARGETS :=
MODULE_ASMTARGETS :=