#!/bin/bash



DIRROOT=ucos
NIOS2COMMANDSHELL=/home/jonah/altera/13.1/nios2eds/nios2_command_shell.sh
SOPC_BUILDER_PATH=$(pwd)/drivers


for i in {0..1} M
do
	${NIOS2COMMANDSHELL} nios2-app-generate-makefile --app-dir ./${DIRROOT}${i} \
	--bsp-dir ${DIRROOT}${i}_bsp --elf-name ${DIRROOT}${i}.elf --no-src
done



