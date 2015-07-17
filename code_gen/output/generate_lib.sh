#!/bin/bash

OUTPUT_DIR=/home/jonah/fingerprinting/code_gen/output
NIOS2COMMANDSHELL=/home/jonah/altera/13.1/nios2eds/nios2_command_shell.sh
LIBDIR=${OUTPUT_DIR}/critical_library

${NIOS2COMMANDSHELL} nios2-lib-generate-makefile --lib-dir ${LIBDIR} 
--lib-name "critical_library" --bps-dir ${OUTPUT_DIR}/cpuM_bsp --src-dir $ {LIBDIR}
