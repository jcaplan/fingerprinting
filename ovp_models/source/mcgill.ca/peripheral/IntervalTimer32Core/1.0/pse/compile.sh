#!/bin/bash

igen.exe --batch pse.tcl --writec pse --writexml periph.xml 
#--overwrite
make NOVLNV=1

# mkdir -p $IMPERAS_HOME/lib/${IMPERAS_ARCH}/ImperasLib/mcgill.ca/peripheral/IntervalTimer32Core/1.0/
# echo 'cp pse.pse $IMPERAS_HOME/lib/${IMPERAS_ARCH}/ImperasLib/mcgill.ca/peripheral/IntervalTimer32Core/1.0/'
# cp pse.pse $IMPERAS_HOME/lib/${IMPERAS_ARCH}/ImperasLib/mcgill.ca/peripheral/IntervalTimer32Core/1.0/
# echo 'cp periph.xml $IMPERAS_HOME/lib/${IMPERAS_ARCH}/ImperasLib/mcgill.ca/peripheral/IntervalTimer32Core/1.0/'
# cp periph.xml $IMPERAS_HOME/lib/${IMPERAS_ARCH}/ImperasLib/mcgill.ca/peripheral/IntervalTimer32Core/1.0/