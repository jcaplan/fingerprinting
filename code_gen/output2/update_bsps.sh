#!/bin/bash

if [ -z "${NIOS_CODEGEN_ROOT}" ]; then
  echo "Environment variable NIOS_CODEGEN_ROOT undefined!"
  exit 1
fi


OUTPUT_DIR=/home/jonah/fingerprinting/code_gen/output2
BSPMOD_DIR=${NIOS_CODEGEN_ROOT}/platform/bsp_mods
OS_DIR=${NIOS_CODEGEN_ROOT}/platform/micrium_uc_osii


for i in {0..3} M
do
	cp -f ${BSPMOD_DIR}/os_cpu_c.c            ${OUTPUT_DIR}/cpu${i}_bsp/HAL/src/
	cp -f ${OS_DIR}/inc/os_cfg.h              ${OUTPUT_DIR}/cpu${i}_bsp/UCOSII/inc/
	cp -f ${OS_DIR}/inc/ucos_ii.h             ${OUTPUT_DIR}/cpu${i}_bsp/UCOSII/inc/
	cp -f ${OS_DIR}/src/os_dbg.c              ${OUTPUT_DIR}/cpu${i}_bsp/UCOSII/src/
	cp -f ${OS_DIR}/src/os_mem.c              ${OUTPUT_DIR}/cpu${i}_bsp/UCOSII/src/
	cp -f ${OS_DIR}/src/os_task.c             ${OUTPUT_DIR}/cpu${i}_bsp/UCOSII/src/
	cp -f ${OS_DIR}/src/os_dbg_r.c            ${OUTPUT_DIR}/cpu${i}_bsp/UCOSII/src/
	cp -f ${OS_DIR}/src/os_mutex.c            ${OUTPUT_DIR}/cpu${i}_bsp/UCOSII/src/
	cp -f ${OS_DIR}/src/os_time.c             ${OUTPUT_DIR}/cpu${i}_bsp/UCOSII/src/
	cp -f ${OS_DIR}/src/os_flag.c             ${OUTPUT_DIR}/cpu${i}_bsp/UCOSII/src/
	cp -f ${OS_DIR}/src/os_q.c                ${OUTPUT_DIR}/cpu${i}_bsp/UCOSII/src/
	cp -f ${OS_DIR}/src/os_tmr.c              ${OUTPUT_DIR}/cpu${i}_bsp/UCOSII/src/
	cp -f ${OS_DIR}/src/os_core.c             ${OUTPUT_DIR}/cpu${i}_bsp/UCOSII/src/
	cp -f ${OS_DIR}/src/os_mbox.c             ${OUTPUT_DIR}/cpu${i}_bsp/UCOSII/src/
	cp -f ${OS_DIR}/src/os_sem.c              ${OUTPUT_DIR}/cpu${i}_bsp/UCOSII/src/
done

for i in {0..3}
do
	cp -f ${BSPMOD_DIR}/alt_exception_entry_gp.S ${OUTPUT_DIR}/cpu${i}_bsp/HAL/src/alt_exception_entry.S
	cp -f ${BSPMOD_DIR}/os_core.c                ${OUTPUT_DIR}/cpu${i}_bsp/UCOSII/src/
	cp -f ${BSPMOD_DIR}/ucos_ii.h                ${OUTPUT_DIR}/cpu${i}_bsp/UCOSII/inc/
done

cp -f ${BSPMOD_DIR}/os_cpu_c_MON.c           ${OUTPUT_DIR}/cpuM_bsp/HAL/src/os_cpu_c.c


# update system.h for monitor bsp
DIR=.tmp
mkdir -p $DIR

# fix the comparator interrupt

sed -r -e 's/(CFPU_0_CSR_IRQ) (-*[0-9]+)/\1 3/' \
		-e 's/(CFPU_0_CSR_IRQ_INTERRUPT_CONTROLLER_ID) (-*[0-9]+)/\1 0/' \
		<${OUTPUT_DIR}/cpuM_bsp/system.h >$DIR/test.x

cp $DIR/test.x ${OUTPUT_DIR}/cpuM_bsp/system.h

#update the Monitor dma interrupts file for all
for i in {0..3}
do
	irq=$(expr $i + 5)
	sed -r -e "s/(PROCESSOR${i}_0_DMA_0_IRQ) (-*[0-9]+)/\1 ${irq}/" \
		-e "s/(PROCESSOR${i}_0_DMA_0_IRQ_INTERRUPT_CONTROLLER_ID) (-*[0-9]+)/\1 0/" \
		<${OUTPUT_DIR}/cpuM_bsp/system.h >$DIR/test.x
	cp $DIR/test.x ${OUTPUT_DIR}/cpuM_bsp/system.h

done


rm -r $DIR

