#!/bin/bash

#--overwrite
make NOVLNV=1
echo 'cp model.so $IMPERAS_HOME/lib/${IMPERAS_ARCH}/ImperasLib/mcgill.ca/processor/nios_ii/1.0/'
cp model.so $IMPERAS_HOME/lib/${IMPERAS_ARCH}/ImperasLib/mcgill.ca/peripheral/tlb/1.0/
