#!/bin/bash
pwd=$(pwd)
fprint_dir=$pwd/../../../quartus/full_system_arria_8_oflow

for i in {0..7} Mon
do
    cd ${fprint_dir}/software/ucos${i}
    make all
done
cd ${fprint_dir}/software/ucosMon
make all
cd $pwd
