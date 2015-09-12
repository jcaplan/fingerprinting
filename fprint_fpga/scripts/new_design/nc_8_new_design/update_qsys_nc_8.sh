#!/bin/bash
fprint_root=../../..
fprint_dir=${fprint_root}/quartus/full_system_arria_8_new_design
hw_dir=${fprint_dir}/nios_fprint/synthesis/submodules

DIR=.tmp
mkdir -p $DIR

for i in {0..7}
do
	file=$hw_dir/nios_fprint_processor${i}_0.v

	string="(\.data_address *\(\))(.*)"
	string=${string}"(\.writedata *\(\))(.*)"
	string=${string}"(\.write *\(\))(.*)"
	string=${string}"(\.waitrequest *\(\))(.*)"

	replace="\.data_address       \(cpu${i}_data_master_address\)\2"
	replace=${replace}"\.writedata          \(cpu${i}_data_master_writedata\)\4"
	replace=${replace}"\.write              \(cpu${i}_data_master_write\)\6"
	replace=${replace}"\.waitrequest        \(cpu${i}_data_master_waitrequest\)\8"
	sed -r "

	/(\.data_address( )*)\((.)*\)/ {
		N 
		/(\.writedata( )*)\((.)*\)/ {
			N
			/(\.write( )*)\((.)*\)/ {
				N
				/(\.waitrequest( )*)\((.)*\)/ {
					N 
					s/${string}/${replace}/
				}		
			}	
		}	
	}
	" <$file >$DIR/test.x
	cp $DIR/test.x $file

done

rm -r $DIR
