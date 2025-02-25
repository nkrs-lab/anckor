# Copyright (c) 2024 niko @assembly-lab

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

MODULE_ID := $(GET_MODULE_ID)
MODULE_DEPS := lib/sys \
			drv \
			arch \
			lib/libc \
			kernel \
			tests

include tools/make/compile.mk