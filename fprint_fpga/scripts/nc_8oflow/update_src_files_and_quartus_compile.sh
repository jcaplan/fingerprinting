#!/bin/bash
fprint_root=~/Documents/SURE/fingerprinting/fprint_fpga
src_dir=${fprint_root}/src
fprint_dir=${fprint_root}/quartus/full_system_arria_8oflow
hw_dir=${fprint_dir}/nios_fprint/synthesis/submodules/
quartus_dir=/CMC/tools/altera/13.1/quartus/bin
out_dir=${fprint_dir}/output_files

stp_filename=stp1.stp
project_name=fprint_1_1
output_filename=${project_name}.sof
stp_output_filename=${project_name}_stp.sof
ecl_output_filename=${project_name}_ecl.sof


cp ${src_dir}/cfpu.v ${hw_dir}
cp ${src_dir}/comparator.v ${hw_dir}
cp ${src_dir}/comp_registers.v ${hw_dir}
cp ${src_dir}/crc_defines.v ${hw_dir}
cp ${src_dir}/csr_registers.v ${hw_dir}
cp ${src_dir}/fifo.v ${hw_dir}
cp ${src_dir}/fprint_registers.v ${hw_dir}
cp ${src_dir}/oflow_registers.v ${hw_dir}

echo with stp
sleep 1

sed -i "s/ENABLE_SIGNALTAP OFF/ENABLE_SIGNALTAP ON/g" ${fprint_dir}/fprint_1_1.qsf
${quartus_dir}/quartus_map --read_settings_files=on --write_settings_files=off --64bit ${fprint_dir}/${project_name} -c ${project_name}
${quartus_dir}/quartus_fit --read_settings_files=off --write_settings_files=off --64bit ${fprint_dir}/${project_name} -c ${project_name}
${quartus_dir}/quartus_asm --read_settings_files=off --write_settings_files=off --64bit ${fprint_dir}/${project_name} -c ${project_name}
${quartus_dir}/quartus_sta --64bit ${fprint_dir}/${project_name} -c ${project_name}
${quartus_dir}/quartus_eda --read_settings_files=off --write_settings_files=off --64bit ${fprint_dir}/${project_name} -c ${project_name}

mv ${out_dir}/${output_filename} ${out_dir}/${stp_output_filename}

clear
echo without stp
sleep 1

sed -i "s/ENABLE_SIGNALTAP ON/ENABLE_SIGNALTAP OFF/g" ${fprint_dir}/fprint_1_1.qsf
${quartus_dir}/quartus_stp ${fprint_dir}/${project_name} --64bit --stp_file ${out_dir}/${stp_filename} --disable
${quartus_dir}/quartus_map --read_settings_files=on --write_settings_files=off --64bit ${fprint_dir}/${project_name} -c ${project_name}
${quartus_dir}/quartus_fit --read_settings_files=off --write_settings_files=off --64bit ${fprint_dir}/${project_name} -c ${project_name}
${quartus_dir}/quartus_asm --read_settings_files=off --write_settings_files=off --64bit ${fprint_dir}/${project_name} -c ${project_name}
${quartus_dir}/quartus_sta --64bit ${fprint_dir}/${project_name} -c ${project_name}
${quartus_dir}/quartus_eda --read_settings_files=off --write_settings_files=off --64bit ${fprint_dir}/${project_name} -c ${project_name}

mv ${out_dir}/${output_filename} ${out_dir}/${ecl_output_filename}
