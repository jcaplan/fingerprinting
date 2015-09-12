#!/bin/bash

fprint_root=../../..
fprint_dir=${fprint_root}/quartus/full_system_arria_8_oflow_nmr




for i in {0..7}
do
    pushd ${fprint_dir}/software/ucos${i}_bsp
    nios2-bsp-generate-files --settings=settings.bsp --bsp-dir=.
    popd

done

pushd ${fprint_dir}/software/ucosMon_bsp
nios2-bsp-generate-files --settings=settings.bsp --bsp-dir=.
popd

sh update_bsps_nc_8.sh
