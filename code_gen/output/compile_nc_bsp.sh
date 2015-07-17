#!/bin/bash


for i in {0..1} M
do
	bsp_dir=cpu${i}_bsp
    nios2-bsp-generate-files --settings=${bsp_dir}/settings.bsp --bsp-dir=${bsp_dir}

done

source update_bsps.sh
