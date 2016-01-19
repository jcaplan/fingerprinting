#!/bin/bash

OUTPUT_DIR=/home/jonah/fingerprinting/code_gen/output
NIOS2COMMANDSHELL=/home/jonah/altera/13.1/nios2eds/nios2_command_shell.sh

${NIOS2COMMANDSHELL} nios2-bsp-update-settings \
--settings ${OUTPUT_DIR}/cpu2_bsp/settings.bsp \
--cmd update_memory_region memory_0_onchip_memoryMain memory_0_onchip_memoryMain 0x96020 0x2ffe0

${NIOS2COMMANDSHELL} nios2-bsp-update-settings \
--settings ${OUTPUT_DIR}/cpu2_bsp/settings.bsp \
--cmd add_memory_region stack_bin_1 memory_0_onchip_memoryMain 0xc6000 0x1000 \
--cmd add_section_mapping .stack_bin_1 stack_bin_1

