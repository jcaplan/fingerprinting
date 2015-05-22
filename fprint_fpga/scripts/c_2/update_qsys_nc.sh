#!/bin/bash
fprint_root=../..
fprint_dir=${fprint_root}/quartus/full_system_arria_c2
hw_dir=${fprint_dir}/nios_fprint/synthesis/submodules

DIR=.tmp
mkdir -p $DIR

for i in {0..1}
do
	file=$hw_dir/nios_fprint_processor${i}_0.v

	string="(\.avs_address_conduit *)(\(\))"
	
	replace="\1 \(cpu${i}_instruction_master_address\)"

	sed -r "s/$string/$replace/" <$file >$DIR/test.x
	cp $DIR/test.x $file

done

#copy crc_defines.v into submodule directory, not done automatically
cp $fprint_root/src/crc_defines.v $hw_dir

rm -r $DIR