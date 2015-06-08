#!/bin/bash

cp -f /data/jcapla9/fingerprinting/fprint_fpga/quartus/full_system_arria_c2/nios_fprint.sopcinfo ..


for i in {0..1} Mon
do
	bsp_dir=ucos${i}_bsp
    nios2-bsp-generate-files --settings=${bsp_dir}/settings.bsp --bsp-dir=${bsp_dir}

done

sh update_bsps_nc.sh