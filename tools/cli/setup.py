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

from setuptools import setup, find_packages
import os

setup(
    name="anckor CLI",
    version="0.0.2",
    description="A swiss-army knife tool for the Anckor project",
    license="SPL-3",

    packages=find_packages(),
    entry_points={
        'console_scripts': (
            'anckor = src.top:main',
        )
    },
)