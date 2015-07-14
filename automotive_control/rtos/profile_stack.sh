#!/bin/bash

${NIOS_CODEGEN_ROOT:?"Need to set NIOS_CODEGEN_ROOT non-empty"}


ROOTNAME=ucos
NIOS2COMMANDSHELL=/home/jonah/altera/13.1/nios2eds/nios2_command_shell.sh


BSPDIR=${ROOTNAME}M_bsp

#assume bsp is already built for now, figure out how to check if they exist
if [ ! -d "${BSPDIR}" ]; then
  echo "Can't do profiling without ${ROOTNAME}M_bsp"
  exit 1
fi

#for each function, copy the ert_main and other stuff into a directory
#create an application, compile, and run the profiler to get the stack height
#then write the output back to the original file!

set -e
function cleanup {
  echo "Removing ./prof"
rm -rf prof
}
trap cleanup EXIT



# mkdir -p prof




ROOT=../CompiledC

DIRLIST=$(find $ROOT  -mindepth 1 -maxdepth 1 -type d)

mkdir -p prof
for dir in $DIRLIST 
do
	echo "copying files in $dir to ./prof"
	find $dir  -regex ".*\.[ch]" -exec cp '{}'  ./prof \;
	SRCLIST=$(find prof -regex ".*\.c")
	echo "compiling project onto monitor core"
	echo "nios2-app-generate-makefile --app-dir ./prof \
	--bsp-dir ${BSPDIR} --elf-name ${ROOTNAME}$M.elf --no-src"
	${NIOS2COMMANDSHELL} nios2-app-generate-makefile --app-dir ./prof \
	--bsp-dir ${BSPDIR} --elf-name ${ROOTNAME}.elf --no-src &> /dev/null

	if [ "$?" !=  "0" ]
	then
		echo "Error generating Makefile for $dir"
		exit 1
	fi

	echo "nios2-app-update-makefile --app-dir prof --add-src-files $SRCLIST --set-user-flags -fomit-frame-pointer --set OBJDUMP_INCLUDE_SOURCE 0"	
	$NIOS2COMMANDSHELL nios2-app-update-makefile --app-dir prof --add-src-files $SRCLIST --set-user-flags -fomit-frame-pointer --set OBJDUMP_INCLUDE_SOURCE 0 &> /dev/null
	if [ "$?" !=  "0" ]
	then
		echo "Error updating makefile for $dir"
		exit 1
	fi

	echo "making project"
	$NIOS2COMMANDSHELL make -C prof	&> /dev/null
	if [ "$?" !=  "0" ]
	then
		echo "Error making project for $dir"
		exit 1
	fi

	echo "now do profiling"
	
	FUNCROOT=$(basename ${dir%_ert_rtw})
	java  -jar ${NIOS_CODEGEN_ROOT}/CodeGen.jar -p --prof-objdump ./prof/ucos.objdump  \
	--root-func ${FUNCROOT}_step --log $dir/profile.log
	if [ "$?" !=  "0" ]
	then
		echo "Error running profiler"
		exit 1
	fi
	echo "cleaning prof folder"
	rm -rf prof/*
done


