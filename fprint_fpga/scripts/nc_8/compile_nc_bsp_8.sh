#!/bin/bash

fprint_root=../..
fprint_dir=${fprint_root}/quartus/full_system_arria_8


for i in {0..8}
do
    cd ${fprint_dir}/software/ucos${i}_bsp
     nios2-bsp-generate-files --settings=settings.bsp --bsp-dir=.

done
cd ${fprint_dir}/software/ucosMon_bsp
nios2-bsp-generate-files --settings=settings.bsp --bsp-dir=.

cd /data/jcapla9/
sh update_bsps_nc_8.sh