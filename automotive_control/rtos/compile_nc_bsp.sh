#!/bin/bash

cp -f $FPRINT_HOME/fprint_fpga/quartus/full_system_arria_c2/nios_fprint.sopcinfo ..


for i in {0..1} M
do
	bsp_dir=ucos${i}_bsp
    nios2-bsp-generate-files --settings=${bsp_dir}/settings.bsp --bsp-dir=${bsp_dir}

done

# sh update_bsps_nc.sh
