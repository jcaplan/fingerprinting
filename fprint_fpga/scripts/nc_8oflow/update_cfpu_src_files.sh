#!/bin/bash
fprint_root=~/Documents/SURE/fingerprinting/fprint_fpga
src_dir=${fprint_root}/src
fprint_dir=${fprint_root}/quartus/full_system_arria_8oflow
hw_dir=${fprint_dir}/nios_fprint/synthesis/submodules/

cp ${src_dir}/cfpu.v ${hw_dir}
cp ${src_dir}/comparator.v ${hw_dir}
cp ${src_dir}/comp_registers.v ${hw_dir}
cp ${src_dir}/crc_defines.v ${hw_dir}
cp ${src_dir}/csr_registers.v ${hw_dir}
cp ${src_dir}/fifo.v ${hw_dir}
cp ${src_dir}/fprint_registers.v ${hw_dir}
cp ${src_dir}/oflow_registers.v ${hw_dir}

echo "done"
