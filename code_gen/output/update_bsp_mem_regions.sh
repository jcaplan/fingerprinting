#!/bin/bash

OUTPUT_DIR=/home/jonah/fingerprinting/code_gen/output
NIOS2COMMANDSHELL=/home/jonah/altera/13.1/nios2eds/nios2_command_shell.sh

${NIOS2COMMANDSHELL} nios2-bsp-update-settings \
--settings ${OUTPUT_DIR}/cpu0_bsp/settings.bsp \
--cmd update_memory_region memory_0_onchip_memoryMain memory_0_onchip_memoryMain 0x64020 0x2efe0

${NIOS2COMMANDSHELL} nios2-bsp-update-settings \
--settings ${OUTPUT_DIR}/cpu0_bsp/settings.bsp \
--cmd add_memory_region stack_bin_1 memory_0_onchip_memoryMain 0x94000 0x1000 \
--cmd add_section_mapping .stack_bin_1 stack_bin_1

${NIOS2COMMANDSHELL} nios2-bsp-update-settings \
--settings ${OUTPUT_DIR}/cpu0_bsp/settings.bsp \
--cmd update_memory_region memory_0_onchip_memoryMain memory_0_onchip_memoryMain 0x64020 0x2efe0

${NIOS2COMMANDSHELL} nios2-bsp-update-settings \
--settings ${OUTPUT_DIR}/cpu0_bsp/settings.bsp \
--cmd add_memory_region stack_bin_2 memory_0_onchip_memoryMain 0x93000 0x1000 \
--cmd add_section_mapping .stack_bin_2 stack_bin_2

${NIOS2COMMANDSHELL} nios2-bsp-update-settings \
--settings ${OUTPUT_DIR}/cpu1_bsp/settings.bsp \
--cmd update_memory_region memory_0_onchip_memoryMain memory_0_onchip_memoryMain 0x32020 0x2efe0

${NIOS2COMMANDSHELL} nios2-bsp-update-settings \
--settings ${OUTPUT_DIR}/cpu1_bsp/settings.bsp \
--cmd add_memory_region stack_bin_1 memory_0_onchip_memoryMain 0x62000 0x1000 \
--cmd add_section_mapping .stack_bin_1 stack_bin_1

${NIOS2COMMANDSHELL} nios2-bsp-update-settings \
--settings ${OUTPUT_DIR}/cpu1_bsp/settings.bsp \
--cmd update_memory_region memory_0_onchip_memoryMain memory_0_onchip_memoryMain 0x32020 0x2efe0

${NIOS2COMMANDSHELL} nios2-bsp-update-settings \
--settings ${OUTPUT_DIR}/cpu1_bsp/settings.bsp \
--cmd add_memory_region stack_bin_2 memory_0_onchip_memoryMain 0x61000 0x1000 \
--cmd add_section_mapping .stack_bin_2 stack_bin_2

