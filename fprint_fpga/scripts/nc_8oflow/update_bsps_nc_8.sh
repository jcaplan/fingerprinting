#!/bin/bash
fprint_root=../..
fprint_dir=${fprint_root}/quartus/full_system_arria_8oflow

programming_dir=${fprint_dir}/output_files
sw_dir=${fprint_dir}/software


for i in {0..7}
do
	cp -f $fprint_root/src/software/bsp_mods/alt_exception_entry.S $sw_dir/ucos${i}_bsp/HAL/src/
	cp -f $fprint_root/src/software/bsp_mods/os_core.c             $sw_dir/ucos${i}_bsp/UCOSII/src/
	cp -f $fprint_root/src/software/bsp_mods/ucos_ii.h             $sw_dir/ucos${i}_bsp/UCOSII/inc/	
done


# update system.h for monitor bsp
DIR=.tmp
mkdir -p $DIR

# fix the comparator interrupt

sed -r -e 's/(CFPU_0_CSR_IRQ) (-*[0-9]+)/\1 3/' \
		-e 's/(CFPU_0_CSR_IRQ_INTERRUPT_CONTROLLER_ID) (-*[0-9]+)/\1 0/' \
		<$sw_dir/ucosMon_bsp/system.h >$DIR/test.x

cp $DIR/test.x $sw_dir/ucosMon_bsp/system.h

#update the Monitor dma interrupts file for all
for i in {0..7}
do
	irq=$(expr $i + 5)
	sed -r -e "s/(PROCESSOR${i}_0_DMA_0_IRQ) (-*[0-9]+)/\1 ${irq}/" \
		-e "s/(PROCESSOR${i}_0_DMA_0_IRQ_INTERRUPT_CONTROLLER_ID) (-*[0-9]+)/\1 0/" \
		<$sw_dir/ucosMon_bsp/system.h >$DIR/test.x
	cp $DIR/test.x $sw_dir/ucosMon_bsp/system.h

done

rm -r $DIR
