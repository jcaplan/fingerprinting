#!/bin/bash

ROOT=ipet
INCLUDE='for_loop_100000_0.h for_loop_100000_0_private.h for_loop_100000_0_types.h'
if [[ $# > 0 ]];then 
	ROOT=$1
fi

nios=~/altera/13.1/nios2eds/nios2_command_shell.sh

${nios} nios2-elf-gcc ${ROOT}.c for_loop_100000_0.c -Wall -o ${ROOT}.out -mhw-div -mhw-mul
${nios} nios2-elf-objdump ${ROOT}.out -d > ${ROOT}.objdump
${nios} nios2-elf-objdump ${ROOT}.out -s -z --section=.wcet_annot > ${ROOT}.annot
