#!/usr/bin/env bash


LIST=`find ./ -name '*.a'`
echo "$LIST"
for archive in $LIST
do

	ar -t $archive > $archive.out

	ar -x $archive 

	OBJDIR=${archive}_obj
	mkdir -p $OBJDIR 
	cp *.o $OBJDIR
	rm *.o

	cd $OBJDIR	

	FILELIST=`find ./ -name '*.o'`
	OUTPUT=../${archive}_functions.txt

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
done

# rm -rf obj/