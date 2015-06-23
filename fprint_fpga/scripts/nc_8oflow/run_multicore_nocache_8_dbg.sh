#!/bin/bash
fprint_root=../..
fprint_dir=${fprint_root}/quartus/full_system_arria_8oflow

programming_dir=${fprint_dir}/output_files
sw_dir=${fprint_dir}/software

pushd $programming_dir
nios2-configure-sof  fprint_1_1_ecl.sof
popd

nios2-download -g --cable=1 --instance=2 ${sw_dir}/ucosMon/ucosMon.elf
sleep 1
nios2-download -g --cable=1 --instance=3 ${sw_dir}/ucos2/ucos2.elf
sleep 1
nios2-download -g --cable=1 --instance=4 ${sw_dir}/ucos3/ucos3.elf
sleep 1
nios2-download -g --cable=1 --instance=5 ${sw_dir}/ucos4/ucos4.elf
sleep 1
nios2-download -g --cable=1 --instance=6 ${sw_dir}/ucos5/ucos5.elf
sleep 1
nios2-download -g --cable=1 --instance=7 ${sw_dir}/ucos6/ucos6.elf
sleep 1
nios2-download -g --cable=1 --instance=8 ${sw_dir}/ucos7/ucos7.elf
sleep 1
nios2-download -g --cable=1 --instance=1 ${sw_dir}/ucos0/ucos0.elf
sleep 1
nios2-download -g --cable=1 --instance=0 ${sw_dir}/ucos1/ucos1.elf

for i in {0..8}
do
	if [ "$i" -ne "8" ]
	then 
		
		if [ "$i" -gt "1" ]
		then
			inst=$((i+1))

		elif [ "$i" -eq "1" ]
		then
			inst=0

		elif [ "$i" -eq "0" ]
		then
			inst=1
		fi

		xterm -e "echo Core $i; nios2-terminal -c 1 -i $inst" &
	else 
		
		inst=2

		xterm -e "echo Monitor; nios2-terminal -c 1 -i $inst" &
	fi

done
