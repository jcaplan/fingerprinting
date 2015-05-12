#!/bin/bash


###############################################################################
# This is the command to generate a new application from preexisting BSP
###############################################################################

nios2-app-generate-makefile --app-dir . --bsp-dir ../ucos0_bsp --elf-name ucos0.elf --no-src

###############################################################################
# This is the command to add new source files to makefile
###############################################################################

nios2-app-update-makefile --app-dir /data/jcapla9/fingerprinting/automotive_control/rtos/ucosMon --add-src-files AirbagModel.h


###############################################################################
# Check that it worked
###############################################################################

nios2-app-update-makefile --list-src-files --app-dir /data/jcapla9/fingerprinting/automotive_control/rtos/ucosMon