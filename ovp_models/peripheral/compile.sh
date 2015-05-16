#!/usr/bin/env bash



LIST=`find ./*/ -name 'compile.sh'`
echo "$LIST"

HOME='pwd'
for f in $LIST
do
	DIR=$(dirname "$f")
	cd $DIR
	BASE=$(basename $f)
	./$BASE
	cd $HOME
done
