#!/bin/bash


if [ -z "${NIOS_CODEGEN_ROOT}" ]; then
  echo "Environment variable NIOS_CODEGEN_ROOT undefined!"
  exit 1
fi

NIOS2COMMANDSHELL=/home/jonah/altera/13.1/nios2eds/nios2_command_shell.sh
SOPC_BUILDER_PATH=${NIOS_CODEGEN_ROOT}/drivers
SOPC_LOCATION=${NIOS_CODEGEN_ROOT}/platform/nios_fprint.sopcinfo
OUTPUT_DIR=/home/jonah/fingerprinting/code_gen/output

for i in {0..1} M
do
	${NIOS2COMMANDSHELL} nios2-bsp ucosii ${OUTPUT_DIR}/cpu${i}_bsp ${SOPC_LOCATION} \
	--cpu-name processor${i}_0_cpu${i}

	if [ "$?" !=  "0" ]
	then
		echo "ERROR: could not generate ${OUTPUT_DIR}/cpu${i}_bsp"
		exit 1
	fi
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
${NIOS2COMMANDSHELL} nios2-bsp-update-settings  --settings ${OUTPUT_DIR}/cpu0_bsp/settings.bsp \
--cmd update_memory_region memory_0_onchip_memoryMain memory_0_onchip_memoryMain 0x64020 0x30fe0 \
--cmd add_memory_region stack_bin_0 memory_0_onchip_memoryMain 0x95000 0x1000 \
--cmd add_section_mapping .stack_bin_0 stack_bin_0 \
--cmd enable_sw_package mem_manager \
--cmd enable_sw_package mpu_utils \
--cmd enable_sw_package fingerprint \
--cmd enable_sw_package reset_monitor \
--cmd enable_sw_package runtime_monitor \
--cmd set_setting hal.make.bsp_cflags_user_flags -fomit-frame-pointer \
--cmd set_setting hal.timestamp_timer processor0_0_timestamp

# cpu1
${NIOS2COMMANDSHELL} nios2-bsp-update-settings  --settings ${OUTPUT_DIR}/cpu1_bsp/settings.bsp \
--cmd update_memory_region memory_0_onchip_memoryMain memory_0_onchip_memoryMain 0x32020 0x30fe0 \
--cmd add_memory_region stack_bin_0 memory_0_onchip_memoryMain 0x63000 0x1000 \
--cmd add_section_mapping .stack_bin_0 stack_bin_0 \
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
--cmd enable_sw_package runtime_monitor \
--cmd set_setting hal.make.bsp_cflags_user_flags -fomit-frame-pointer \
--cmd set_setting hal.timestamp_timer processor1_0_timestamp

# cpuM
${NIOS2COMMANDSHELL} nios2-bsp-update-settings  --settings ${OUTPUT_DIR}/cpuM_bsp/settings.bsp \
--cmd update_memory_region memory_0_onchip_memoryMain memory_0_onchip_memoryMain 0x20 0x30fe0 \
--cmd add_memory_region global_data memory_0_onchip_memoryMain 0x31000 0x1000 \
--cmd add_section_mapping .global_data global_data \
--cmd add_section_mapping .critical processorM_0_scratchpad \
--cmd add_section_mapping .shared shared_memory \
--cmd enable_sw_package repos  \
--cmd enable_sw_package mpu_utils \
--cmd enable_sw_package fingerprint \
--cmd enable_sw_package reset_monitor \
--cmd enable_sw_package runtime_monitor \
--cmd enable_sw_package dma \
--cmd set_setting hal.make.bsp_cflags_user_flags -fomit-frame-pointer \
--cmd set_setting hal.timestamp_timer processorM_0_timestamp


for i in {0..1} M
do
	 ${NIOS2COMMANDSHELL} nios2-bsp-generate-files --bsp-dir ${OUTPUT_DIR}/cpu${i}_bsp \
	 --settings ${OUTPUT_DIR}/cpu${i}_bsp/settings.bsp 
done
	
source ${OUTPUT_DIR}/update_bsps.sh

