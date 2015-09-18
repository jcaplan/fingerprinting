#!/bin/bash

nios=~/altera/13.1/nios2eds/nios2_command_shell.sh

${nios} nios2-elf-gcc ipet.c 
${nios} nios2-elf-objdump a.out -d > ipet.objdump
${nios} nios2-elf-objdump a.out -s -z --section=.wcet_annot > ipet.annot

