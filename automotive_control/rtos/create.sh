#!/bin/bash


DIRROOT=ucos
NIOS2COMMANDSHELL=/home/jonah/altera/13.1/nios2eds/nios2_command_shell.sh
SOPC_BUILDER_PATH=$(pwd)/drivers


for i in {0..1} M
do
	${NIOS2COMMANDSHELL} nios2-bsp ucosii ./${DIRROOT}${i}_bsp ../nios_fprint.sopcinfo \
	--cpu-name processor${i}_0_cpu${i}
	
	# ${NIOS2COMMANDSHELL} nios2-app-generate-makefile --app-dir ./${DIRROOT}${i} \
	# --bsp-dir ${DIRROOT}${i}_bsp --elf-name ${DIRROOT}${i}.elf --no-src
done




# #update linker scripts:
# # update_memory_region Positional Arguments:
# # <name>                  String with the name of the memory region to update.
# # <slave desc>            String with the slave descriptor of the memory device
# #                         for this region.
# # <offset>                String with the byte offset of the memory region from
# #                         the memory device base address.
# # <span>                  String with the span of the memory region in bytes.

# cpu0
${NIOS2COMMANDSHELL} nios2-bsp-update-settings  --settings ${DIRROOT}0_bsp/settings.bsp \
--cmd update_memory_region memory_0_onchip_memoryMain memory_0_onchip_memoryMain 0x64020 0x30fe0 \
--cmd add_memory_region stack_bin memory_0_onchip_memoryMain 0x95000 0x1000 \
--cmd add_section_mapping .stack_bin stack_bin \
--cmd enable_sw_package mem_manager \
--cmd enable_sw_package mpu_utils \
--cmd enable_sw_package fingerprint \
--cmd enable_sw_package reset_monitor \
--cmd set_setting hal.make.bsp_cflags_user_flags -fomit-frame-pointer



# cpu1
${NIOS2COMMANDSHELL} nios2-bsp-update-settings  --settings ${DIRROOT}1_bsp/settings.bsp \
--cmd update_memory_region memory_0_onchip_memoryMain memory_0_onchip_memoryMain 0x32020 0x30fe0 \
--cmd add_memory_region stack_bin memory_0_onchip_memoryMain 0x63000 0x1000 \
--cmd add_section_mapping .stack_bin stack_bin \
--cmd update_section_mapping .bss memory_0_onchip_memoryMain \
--cmd update_section_mapping .text memory_0_onchip_memoryMain \
--cmd update_section_mapping .stack memory_0_onchip_memoryMain \
--cmd update_section_mapping .heap memory_0_onchip_memoryMain \
--cmd update_section_mapping .rodata memory_0_onchip_memoryMain \
--cmd update_section_mapping .rwdata memory_0_onchip_memoryMain \
--cmd enable_sw_package mem_manager \
--cmd enable_sw_package mpu_utils \
--cmd enable_sw_package fingerprint \
--cmd enable_sw_package reset_monitor \
--cmd set_setting hal.make.bsp_cflags_user_flags -fomit-frame-pointer



# cpum
${NIOS2COMMANDSHELL} nios2-bsp-update-settings  --settings ${DIRROOT}M_bsp/settings.bsp \
--cmd update_memory_region memory_0_onchip_memoryMain memory_0_onchip_memoryMain 0x20 0x30fe0 \
--cmd add_memory_region global_data memory_0_onchip_memoryMain 0x31000 0x1000 \
--cmd add_section_mapping .global_data global_data \
--cmd add_section_mapping .critical processorM_0_scratchpad \
--cmd add_section_mapping .shared shared_memory \
--cmd enable_sw_package repos  \
--cmd enable_sw_package mpu_utils \
--cmd enable_sw_package fingerprint \
--cmd enable_sw_package reset_monitor \
--cmd set_setting hal.make.bsp_cflags_user_flags -fomit-frame-pointer


for i in {0..1} M
do
	 ${NIOS2COMMANDSHELL} nios2-bsp-generate-files --bsp-dir ${DIRROOT}${i}_bsp \
	 --settings ${DIRROOT}${i}_bsp/settings.bsp 
done
	
source update_bsps_nc.sh
