#!/bin/bash

if [ -z "${NIOS_CODEGEN_ROOT}" ]; then
  echo "Environment variable NIOS_CODEGEN_ROOT undefined!"
  exit 1
fi


OUTPUT_DIR=/home/jonah/fingerprinting/code_gen/output
BSPMOD_DIR=${NIOS_CODEGEN_ROOT}/platform/bsp_mods


cp -f ${BSPMOD_DIR}/alt_exception_entry_gp.S ${OUTPUT_DIR}/cpu0_bsp/HAL/src/alt_exception_entry.S
cp -f ${BSPMOD_DIR}/alt_exception_entry_gp.S ${OUTPUT_DIR}/cpu1_bsp/HAL/src/alt_exception_entry.S
cp -f ${BSPMOD_DIR}/os_core.c             ${OUTPUT_DIR}/cpu0_bsp/UCOSII/src/
cp -f ${BSPMOD_DIR}/os_core.c             ${OUTPUT_DIR}/cpu1_bsp/UCOSII/src/
cp -f ${BSPMOD_DIR}/ucos_ii.h             ${OUTPUT_DIR}/cpu0_bsp/UCOSII/inc/
cp -f ${BSPMOD_DIR}/ucos_ii.h             ${OUTPUT_DIR}/cpu1_bsp/UCOSII/inc/
cp -f ${BSPMOD_DIR}/os_cpu_c.c            ${OUTPUT_DIR}/cpuM_bsp/HAL/src/

# update system.h for monitor bsp
DIR=.tmp
mkdir -p $DIR

# fix the comparator interrupt

sed -r -e 's/(CFPU_0_CSR_IRQ) (-*[0-9]+)/\1 3/' \
		-e 's/(CFPU_0_CSR_IRQ_INTERRUPT_CONTROLLER_ID) (-*[0-9]+)/\1 0/' \
		<${OUTPUT_DIR}/cpuM_bsp/system.h >$DIR/test.x

cp $DIR/test.x ${OUTPUT_DIR}/cpuM_bsp/system.h

#update the Monitor dma interrupts file for all
for i in {0..1}
do
	irq=$(expr $i + 5)
	sed -r -e "s/(PROCESSOR${i}_0_DMA_0_IRQ) (-*[0-9]+)/\1 ${irq}/" \
		-e "s/(PROCESSOR${i}_0_DMA_0_IRQ_INTERRUPT_CONTROLLER_ID) (-*[0-9]+)/\1 0/" \
		<${OUTPUT_DIR}/cpuM_bsp/system.h >$DIR/test.x
	cp $DIR/test.x ${OUTPUT_DIR}/cpuM_bsp/system.h

done


rm -r $DIR

