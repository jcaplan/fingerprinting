#!/bin/bash
pwd=$(pwd)
fprint_dir=$pwd/../../quartus/full_system_arria_2

for i in {0..1} Mon
do
    cd ${fprint_dir}/software/ucos${i}
    make all
done
cd ${fprint_dir}/software/ucosMon
make all
