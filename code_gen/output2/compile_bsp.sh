#!/bin/bash
OUTPUT_DIR=/home/jonah/fingerprinting/code_gen/output2
NIOS2COMMANDSHELL=/home/jonah/altera/13.1/nios2eds/nios2_command_shell.sh


for i in {0..3} M
do
	bsp_dir=${OUTPUT_DIR}/cpu${i}_bsp
    ${NIOS2COMMANDSHELL} nios2-bsp-generate-files --settings=${bsp_dir}/settings.bsp --bsp-dir=${bsp_dir}

done

source ${OUTPUT_DIR}/update_bsps.sh

