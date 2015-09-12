#!/bin/bash
fprint_root=../../..
fprint_dir=${fprint_root}/quartus/full_system_arria_8_oflow_nmr
hw_dir=${fprint_dir}/nios_fprint/synthesis/submodules

DIR=.tmp
mkdir -p $DIR

for i in {0..7}
do
	file=$hw_dir/nios_fprint_processor${i}_0.v

	string="(\.waitrequest *\(\))(.*)"
	string=${string}"(\.write *\(\))(.*)"
	string=${string}"(\.writedata *\(\))(.*)"	
	string=${string}"(\.data_address *\(\))(.*)"

	replace="\.waitrequest        \(cpu${i}_data_master_waitrequest\)\2"
	replace=${replace}"\.write              \(cpu${i}_data_master_write\)\4"
	replace=${replace}"\.writedata          \(cpu${i}_data_master_writedata\)\6"
	replace=${replace}"\.data_address       \(cpu${i}_data_master_address\)\8"



	sed -r "

	/(\.waitrequest( )*)\((.)*\)/ {
		N 
		/(\.write( )*)\((.)*\)/ {
			N
			/(\.writedata( )*)\((.)*\)/ {
				N
				/(\.data_address( )*)\((.)*\)/ {
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
