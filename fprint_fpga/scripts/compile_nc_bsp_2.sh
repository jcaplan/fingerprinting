#!/bin/bash

fprint_dir=../quartus/full_system_arria_2


for i in {0..1}
do
    cd ${fprint_dir}/software/ucos${i}_bsp
     nios2-bsp-generate-files --settings=settings.bsp --bsp-dir=.

done
cd ${fprint_dir}/software/ucosMon_bsp
nios2-bsp-generate-files --settings=settings.bsp --bsp-dir=.

cd /data/jcapla9/
sh update_bsps_nc_2.sh