#!/bin/bash

mkdir -p tmp
ROOTDIR=`pwd`


###############################################################################
# This is an example of how to compile and create the disassembly.
# nios2-elf-gcc and nios2-elf-objdump must be on the path.
###############################################################################

cd ../automotive_control/CompiledC/

CP_LIST=`find $1 -name '*_ert_rtw'`

for d in $CP_LIST
do
	cd $d
	BASE=$(basename $d)
	BASE=${BASE%_ert_rtw}
	f=${BASE}.objdump
	make all
	nios2-elf-objdump  -dt $BASE > $f 
	cp $f $ROOTDIR/tmp
	make clean &> /dev/null
	echo "Generated $f"
	cd ..
done

cd $ROOTDIR


###############################################################################
# This is how to feed the disassembly file into the CodeGen app
###############################################################################


CP_LIST=`find tmp -name '*.objdump'`
for f in $CP_LIST
do
	NAME=$(basename $f)
	echo "$NAME"
	NAME=${NAME%.objdump}
	DIR=$(dirname "${f}")
	java  -jar CodeGen.jar $f ${NAME}_step
	# dot -Tpdf $f.dot -o tmp/${NAME}.pdf
done



