#!/bin/bash

DIRLIST=$(ls -d */)


for DIR in ${DIRLIST}
do
	pushd $DIR

	FILELIST=$(ls)
	echo $FILELIST
	for FILE in $FILELIST
	do
		extension="${FILE##*.}"
		echo "extension : $extension"
		if [[ $extension != "c" ]] && [[ $extension != "h" ]];  
		then
			# rm $FILE
			echo "removing $FILE"
			rm $FILE
		fi
	done
	
	popd 
done