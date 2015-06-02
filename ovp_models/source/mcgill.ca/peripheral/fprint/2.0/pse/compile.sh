#!/bin/bash

igen.exe --batch fprint.tcl --writec fprint --writexml periph.xml
make NOVLNV=1

mkdir -p $IMPERAS_HOME/lib/${IMPERAS_ARCH}/ImperasLib/mcgill.ca/peripheral/fprint/2.0/
echo 'cp pse.pse $IMPERAS_HOME/lib/${IMPERAS_ARCH}/ImperasLib/mcgill.ca/peripheral/fprint/2.0/'
cp pse.pse $IMPERAS_HOME/lib/${IMPERAS_ARCH}/ImperasLib/mcgill.ca/peripheral/fprint/2.0/
echo 'cp periph.xml $IMPERAS_HOME/lib/${IMPERAS_ARCH}/ImperasLib/mcgill.ca/peripheral/fprint/2.0/'
cp periph.xml $IMPERAS_HOME/lib/${IMPERAS_ARCH}/ImperasLib/mcgill.ca/peripheral/fprint/2.0/