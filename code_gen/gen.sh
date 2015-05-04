#!/bin/bash

mkdir -p tmp
ROOTDIR=`pwd`

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

CP_LIST=`find tmp -name '*.objdump'`
for f in $CP_LIST
do
	NAME=$(basename $f)
	echo "$NAME"
	NAME=${NAME%.objdump}
	java  -jar CodeGen.jar $f ${NAME}_step
	dot -Tpdf cfg.dot -o cfg.pdf
done



