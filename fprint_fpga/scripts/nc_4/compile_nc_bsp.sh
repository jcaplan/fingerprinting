#!/bin/bash
pwd=$(pwd)
fprint_dir=$pwd/../../quartus/full_system_arria_4

for i in {0..3} Mon
do
	bsp_dir=${fprint_dir}/software/ucos${i}_bsp
    nios2-bsp-generate-files --settings=${bsp_dir}/settings.bsp --bsp-dir=${bsp_dir}

done

sh update_bsps_nc_4.sh