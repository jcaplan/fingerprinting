#!/bin/bash
fprint_root=../..
fprint_dir=${fprint_root}/quartus/full_system_arria_2
hw_dir=${fprint_dir}/nios_fprint/synthesis/submodules

DIR=.tmp
mkdir -p $DIR

for i in {0..1}
do
	file=$hw_dir/nios_fprint_processor${i}_0.v

	string="(\.waitrequest *\(\))(.*)"
	string=${string}"(\.data_address *\(\))(.*)"
	string=${string}"(\.writedata *\(\))(.*)"
	string=${string}"(\.write *\(\))(.*)"
	replace="\.waitrequest        \(cpu${i}_data_master_waitrequest\)\2"
	replace=${replace}"\.data_address       \(cpu${i}_data_master_address\)\4"
	replace=${replace}"\.writedata          \(cpu${i}_data_master_writedata\)\6"
	replace=${replace}"\.write              \(cpu${i}_data_master_write\)\8"
	sed -r "
	/(\.waitrequest( )*)\((.)*\)/ {
		N 
		/(\.data_address( )*)\((.)*\)/ {
			N 
			/(\.writedata( )*)\((.)*\)/ {
				N
				/(\.write( )*)\((.)*\)/ {
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