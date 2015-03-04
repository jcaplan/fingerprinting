#!/bin/bash
fprint_root=../..
fprint_dir=${fprint_root}/quartus/full_system_arria_2
sw_dir=${fprint_dir}/software

DIR=.tmp
mkdir -p $DIR


#update the settings.bsp file for all

for i in {0..1} Mon
do
	pwd=$(pwd)
	file=$sw_dir/ucos${i}_bsp/settings.bsp
	sed -r -e "s:(<BspGeneratedLocation>)(.)*(quartus/full_system_arria_2/software/ucos${i}_bsp</BspGeneratedLocation>):\1${pwd}/../../\3:" \
	-e "s:(<BspSettingsFile>)(.)*(quartus/full_system_arria_2/software/ucos${i}_bsp/settings.bsp</BspSettingsFile>):\1${pwd}/../../\3:" \
	<$file >$DIR/test.x
	cp $DIR/test.x $sw_dir/ucos${i}_bsp/settings.bsp
done


rm -r $DIR
