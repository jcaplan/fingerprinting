#!/bin/bash



OUTPUT_DIR=/home/jonah/fingerprinting/code_gen/output
NIOS2COMMANDSHELL=/home/jonah/altera/13.1/nios2eds/nios2_command_shell.sh


for i in {0..1} M
do
	BSPDIR=${OUTPUT_DIR}/cpu${i}_bsp
	APPDIR=${OUTPUT_DIR}/cpu${i}

	SRCLIST=$(find ${APPDIR} -regex ".*\.c")
	${NIOS2COMMANDSHELL} nios2-app-generate-makefile --app-dir ${APPDIR} \
	--bsp-dir ${BSPDIR} --elf-name cpu${i}.elf --src-files \
 	$SRCLIST --set APP_CFLAGS_USER_FLAGS -fomit-frame-pointer --set OBJDUMP_INCLUDE_SOURCE 0 \
 	--use-lib-dir ${OUTPUT_DIR}/critical_library\
	--set APP_CFLAGS_DEBUG_LEVEL "-g -gdwarf-2" \
	--set APP_INCLUDE_DIRS ${OUTPUT_DIR}/critical_library

	if [ "$?" !=  "0" ]
	then
		echo "Error generating Makefile for ${APPDIR}"
		exit 1
	fi

done

