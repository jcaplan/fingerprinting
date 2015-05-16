#!/bin/bash

igen.exe --batch dma.tcl --writec dma --writexml periph.xml 
#--overwrite
make NOVLNV=1

mkdir -p $IMPERAS_HOME/lib/Linux32/ImperasLib/mcgill.ca/peripheral/dma/1.0/
echo 'cp pse.pse $IMPERAS_HOME/lib/Linux32/ImperasLib/mcgill.ca/peripheral/dma/1.0/'
cp pse.pse $IMPERAS_HOME/lib/Linux32/ImperasLib/mcgill.ca/peripheral/dma/1.0/
echo 'cp periph.xml $IMPERAS_HOME/lib/Linux32/ImperasLib/mcgill.ca/peripheral/dma/1.0/'
cp periph.xml $IMPERAS_HOME/lib/Linux32/ImperasLib/mcgill.ca/peripheral/dma/1.0/