#!/bin/bash
fprint_dir=../../quartus/full_system_arria_2oflow

programming_dir=${fprint_dir}/output_files
sw_dir=${fprint_dir}/software

ret_dir=$(pwd)

#cd $programming_dir
#nios2-configure-sof  fprint_1_1.sof

cd $ret_dir

nios2-download -g --cable=1 --instance=0 ${sw_dir}/ucos1/ucos1.elf
nios2-download -g --cable=1 --instance=1 ${sw_dir}/ucos0/ucos0.elf
nios2-download -g --cable=1 --instance=2 ${sw_dir}/ucosMon/ucosMon.elf



nios2-terminal -c 1 -i 2
