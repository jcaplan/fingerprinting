#!/bin/bash
fprint_root=../../fprint_fpga
sw_dir=.


cp -f bsp_mods/alt_exception_entry_gp.S $sw_dir/ucos0_bsp/HAL/src/alt_exception_entry.S
cp -f bsp_mods/alt_exception_entry_gp.S $sw_dir/ucos1_bsp/HAL/src/alt_exception_entry.S
cp -f bsp_mods/os_core.c             $sw_dir/ucos0_bsp/UCOSII/src/
cp -f bsp_mods/os_core.c             $sw_dir/ucos1_bsp/UCOSII/src/
cp -f bsp_mods/ucos_ii.h             $sw_dir/ucos0_bsp/UCOSII/inc/
cp -f bsp_mods/ucos_ii.h             $sw_dir/ucos1_bsp/UCOSII/inc/


# update system.h for monitor bsp
DIR=.tmp
mkdir -p $DIR

# fix the comparator interrupt

sed -r -e 's/(CFPU_0_CSR_IRQ) (-*[0-9]+)/\1 3/' \
		-e 's/(CFPU_0_CSR_IRQ_INTERRUPT_CONTROLLER_ID) (-*[0-9]+)/\1 0/' \
		<$sw_dir/ucosM_bsp/system.h >$DIR/test.x

cp $DIR/test.x $sw_dir/ucosM_bsp/system.h

#update the Monitor dma interrupts file for all
for i in {0..1}
do
	irq=$(expr $i + 5)
	sed -r -e "s/(PROCESSOR${i}_0_DMA_0_IRQ) (-*[0-9]+)/\1 ${irq}/" \
		-e "s/(PROCESSOR${i}_0_DMA_0_IRQ_INTERRUPT_CONTROLLER_ID) (-*[0-9]+)/\1 0/" \
		<$sw_dir/ucosM_bsp/system.h >$DIR/test.x
	cp $DIR/test.x $sw_dir/ucosM_bsp/system.h

done


rm -r $DIR
