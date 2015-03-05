#!/bin/bash
pwd=$(pwd)
fprint_dir=$pwd/../../quartus/full_system_arria_4

for i in {0..3} Mon
do
    cd ${fprint_dir}/software/ucos${i}
    make all
done
cd ${fprint_dir}/software/ucosMon
make all
cd $pwd