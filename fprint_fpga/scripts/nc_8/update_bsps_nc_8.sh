#!/bin/bash
fprint_root=/data/jcapla9/fprint_ptype_1_2
fprint_dir=${fprint_root}/quartus/full_system_arria_8

programming_dir=${fprint_dir}/output_files
sw_dir=${fprint_dir}/software


cp -f $fprint_root/src/software/bsp_mods/alt_exception_entry.S $sw_dir/ucos0_bsp/HAL/src/
cp -f $fprint_root/src/software/bsp_mods/alt_exception_entry.S $sw_dir/ucos1_bsp/HAL/src/
cp -f $fprint_root/src/software/bsp_mods/alt_exception_entry.S $sw_dir/ucos2_bsp/HAL/src/
cp -f $fprint_root/src/software/bsp_mods/alt_exception_entry.S $sw_dir/ucos3_bsp/HAL/src/
cp -f $fprint_root/src/software/bsp_mods/alt_exception_entry.S $sw_dir/ucos4_bsp/HAL/src/
cp -f $fprint_root/src/software/bsp_mods/alt_exception_entry.S $sw_dir/ucos5_bsp/HAL/src/
cp -f $fprint_root/src/software/bsp_mods/alt_exception_entry.S $sw_dir/ucos6_bsp/HAL/src/
cp -f $fprint_root/src/software/bsp_mods/alt_exception_entry.S $sw_dir/ucos7_bsp/HAL/src/
cp -f $fprint_root/src/software/bsp_mods/os_core.c             $sw_dir/ucos0_bsp/UCOSII/src/
cp -f $fprint_root/src/software/bsp_mods/os_core.c             $sw_dir/ucos1_bsp/UCOSII/src/
cp -f $fprint_root/src/software/bsp_mods/os_core.c             $sw_dir/ucos2_bsp/UCOSII/src/
cp -f $fprint_root/src/software/bsp_mods/os_core.c             $sw_dir/ucos3_bsp/UCOSII/src/
cp -f $fprint_root/src/software/bsp_mods/os_core.c             $sw_dir/ucos4_bsp/UCOSII/src/
cp -f $fprint_root/src/software/bsp_mods/os_core.c             $sw_dir/ucos5_bsp/UCOSII/src/
cp -f $fprint_root/src/software/bsp_mods/os_core.c             $sw_dir/ucos6_bsp/UCOSII/src/
cp -f $fprint_root/src/software/bsp_mods/os_core.c             $sw_dir/ucos7_bsp/UCOSII/src/

