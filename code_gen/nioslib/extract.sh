#!/usr/bin/env bash

ar -t libgcc.a > libgcc.a.out

ar -x libgcc.a 

mkdir -p obj
cp *.o obj/
rm *.o

cd obj	

FILELIST=`find ./ -name '*.o'`
OUTPUT=../libfunctions.txt

rm -f $OUTPUT

for f in $FILELIST
do
	BASE=$(basename $f)
	printf "$BASE\n" >> $OUTPUT
	nios2-elf-objdump -d $f > $f.objdump
	grep "<.*>:" $f.objdump | sed -r 's/.*<(.*)>:/\1/' >> $OUTPUT
	printf "\n" >> $OUTPUT
done

cd ..

rm -rf obj/