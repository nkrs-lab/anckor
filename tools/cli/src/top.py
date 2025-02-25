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

import argparse
import sys
import os
from datetime import datetime
import git

__version__ = "x.x.x"

# *******************************************************************************
# @brief convert the .config file to config.mk
# @param None
# @return None
# *******************************************************************************
def convert_config_to_make():
    # convert the .config file to a config.mk file
    if os.path.isdir("tools/generated"):
        os.system("rm -r tools/generated")
        os.mkdir("tools/generated")
    else:
        os.mkdir("tools/generated")

    input_file = open(".config", "r")
    output_file = open("tools/generated/config.mk", "w+")

    # create a list with targets included in the kernel
    part_list = "GLOBAL_PART_LIST := lib/libc drv/uart "
    for line in input_file:
        line = line.lower()
        if "partition" in line:
            if "=y" in line:
                line = line.replace("config_partition_", '')
                index = line.rfind("=y")
                line = line[:index]
                line = line.replace("_", "/")

                part_list = part_list + line + ' '
    part_list = part_list + '\r\n'
    output_file.write(part_list)

    # reset the cursor atht he beginning of the file
    input_file.seek(0)

    # create a list with modules compiled independently
    module_list = "GLOBAL_MODULE_LIST := "
    for line in input_file:
        line = line.lower()
        if "module" in line:
            if "=y" in line:
                line = line.replace("config_module_", '')
                index = line.rfind("=y")
                line = line[:index]
                line = line.replace("_", "/")

                module_list = module_list + line + ' '
    module_list = module_list + '\r\n'
    output_file.write(module_list)

    # close generated files
    input_file.close()
    output_file.close()
    print("generate config.mk file")

# *******************************************************************************
# @brief create a config.mk file from a _defconfig filye
# @param None
# @return None
# *******************************************************************************
def configure(args):
    print("[CONFIGURE]")

    # copy the specified configuration file
    os.system("cp tools/defconfig/" + args.config + " .config")
    print("use config: " + args.config)

    convert_config_to_make()

# *******************************************************************************
# @brief create a config.mk file through a menuconfig gui tool
# @param None
# @return None
# *******************************************************************************
def menuconfig(args):
    print("[CONFIGURE]")

    # copy the default configuration file
    os.system("python3 tools/cli/src/menuconfig.py")

    convert_config_to_make()

# *******************************************************************************
# @brief delete build directory and all build artifacts
# @param None
# @return None
# *******************************************************************************
def clean(args):
    print("[CLEAN]")

    os.system('make -f tools/make/build.mk clean')

# *******************************************************************************
# @brief call make to build the kernel sources
# @param None
# @return None
# *******************************************************************************
def build(args):
    # add debug flag in debug.mk if needed
    if not os.path.isdir("tools/generated"):
        os.mkdir("tools/generated")

    # open in append mode to not overwrite content
    output_file = open("tools/generated/debug.mk", "w+")
    
    if args.debug:
        print("[BUILD] --debug")

        os.system('make -f tools/make/build.mk setup_build_dir')

        os.system('make -f tools/make/build.mk build BUILD_CORE=true DEBUG_FLAG=true')

        os.system('make -f tools/make/build.mk build BUILD_PARTITION=true DEBUG_FLAG=true')

        output_file.write("DEBUG_FLAG=true")
    else:
        print("[BUILD] --release")

        os.system('make -f tools/make/build.mk setup_build_dir')

        os.system('make -f tools/make/build.mk build BUILD_CORE=true')

        os.system('make -f tools/make/build.mk build BUILD_PARTITION=true')

        output_file.write("DEBUG_FLAG=false")

    os.system('make -f tools/make/build.mk generate_kernel_img')
        
    output_file.close()
# *******************************************************************************
# @brief run the kernel on the configured target
# @param None
# @return None
# *******************************************************************************
def run(args):
    os.system('make -f tools/make/run.mk run')

# *******************************************************************************
# @brief find the root directory absolute path
# @param None
# @return None
# *******************************************************************************
def get_root_dir():
    # get the current absolute path
    root_dir = os.getcwd()

    return root_dir

# *******************************************************************************
# @brief update version and date in a version.h file
# @param None
# @return None
# *******************************************************************************
def update_config():
    root_dir = get_root_dir()
    version_file_name = os.path.join(root_dir, 'config.h')
    # open the file in 'append' mode
    version_file = open(version_file_name, 'w')
    # add a new line 
    version_file.write("\n")
    version_file.write("#define BUILD_DATE ")

    # get the date and write it to the file
    date = datetime.today().strftime('%Y-%m-%d')
    version_file.write('"')
    version_file.write(date)
    version_file.write('"')
    version_file.write("\n")
    version_file.write("#define BUILD_HOUR ")
    hour = datetime.today().strftime('%H:%M:%S')
    version_file.write('"')
    version_file.write(hour)
    version_file.write('"')
    version_file.write("\n")

    global __version__
    version_file.write("#define BUILD_VERSION ")
    repo = git.Repo(root_dir)
    tag_list=repo.git.ls_remote("--tags", "origin")
    # get the last element of a list
    latest_tag = tag_list.split('\n')[-1].split('/')[-1]
    __version__ = str(latest_tag)
    version_file.write('"')
    version_file.write(str(latest_tag))
    version_file.write('"')


# *******************************************************************************
# @brief manage all subcommands
# @param detect function arguments : -p
# @return None
# *******************************************************************************
def options():
    parser = argparse.ArgumentParser(
        description='Anckor command line interface')

    # create a version command
    parser.add_argument('-v', '--version',
                    help='show the current kernel version',
                    action='version',
                    version = "Anckor project " + __version__)

    # create subcommands
    subparsers = parser.add_subparsers()

    # declare "configure" subcommand
    config_parser = subparsers.add_parser('configure',
                                         help='create a .config file from a _defconfig one')
    config_parser.add_argument('--config',
                                help='select the _defconfig file to use',
                                default='default_defconfig')
    config_parser.set_defaults(func=configure)

    # declare "menuconfig" subcommand
    menu_parser = subparsers.add_parser('menuconfig',
                                         help='use a menuconfig tool to create a .config file')
    menu_parser.set_defaults(func=menuconfig)

    # declare "clean" subcommand
    clean_parser = subparsers.add_parser('clean',
                                         help='delete all build artifacts')
    clean_parser.set_defaults(func=clean)

    # declare "build" subcommand
    build_parser = subparsers.add_parser('build',
                                         help='compile and link the kernel')
    build_parser.add_argument('--debug', action='store_true', help='Build kernel in debug mode.')
    build_parser.set_defaults(func=build)

    # declare "run" subcommand
    run_parser = subparsers.add_parser('run',
                                         help='run the kernel on the target')
    run_parser.set_defaults(func=run)

    return parser

# *******************************************************************************
# @brief parse arguments and launch subcommand function
# @param None
# @return None
# *******************************************************************************
def main():
    # update build version and date
    update_config()

    # declare options of the CLI
    parser = options()
    if len(sys.argv) > 1:
        # parse options
        args = parser.parse_args()
        # execute CLI subcommand
        args.func(args)
    else:
        print("not enough arguments")
        print("please check how to use this tool with 'anckor -h' or 'anckor --help'")    

if __name__ == '__main__':
    sys.exit(main())