#!/bin/bash
fprint_root=../../..
src_dir=${fprint_root}/src/new_design

fprint_dir=${fprint_root}/quartus/full_system_arria_8_new_design
hw_dir=${fprint_dir}/nios_fprint/synthesis/submodules/

cp ${src_dir}/defines.v ${hw_dir}

cp ${src_dir}/comparator/cfpu.v ${hw_dir}
cp ${src_dir}/comparator/comparator.v ${hw_dir}
cp ${src_dir}/comparator/csr_registers.v ${hw_dir}
cp ${src_dir}/comparator/fprint_registers.v ${hw_dir}
cp ${src_dir}/comparator/checkpoint.v ${hw_dir}

cp ${src_dir}/fprint/fingerprint.v ${hw_dir}
cp ${src_dir}/fprint/counter.v ${hw_dir}
cp ${src_dir}/fprint/fsm_rtla.v ${hw_dir}
cp ${src_dir}/fprint/pause.v ${hw_dir}
cp ${src_dir}/fprint/rtla.v ${hw_dir}
cp ${src_dir}/fprint/store_buffer.v ${hw_dir}
cp ${src_dir}/fprint/sb_fsm.v ${hw_dir}

echo "src files updated"
