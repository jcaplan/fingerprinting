#!/bin/bash

ROOT=ipet

if [[ $# > 1 ]];then 
	ROOT=$1
fi

nios=~/altera/13.1/nios2eds/nios2_command_shell.sh

${nios} nios2-elf-gcc ${ROOT}.c -O -o ${ROOT}.out
${nios} nios2-elf-objdump ${ROOT}.out -d > ${ROOT}.objdump
${nios} nios2-elf-objdump ${ROOT}.out -s -z --section=.wcet_annot > ${ROOT}.annot
