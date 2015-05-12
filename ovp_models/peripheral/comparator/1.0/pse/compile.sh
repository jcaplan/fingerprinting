#!/bin/bash

igen.exe --batch comparator.tcl --writec comparator --writexml periph.xml 
#--overwrite
make NOVLNV=1
echo 'cp pse.pse $IMPERAS_HOME/lib/Linux32/ImperasLib/mcgill.ca/peripheral/comparator/1.0/'
cp pse.pse $IMPERAS_HOME/lib/Linux32/ImperasLib/mcgill.ca/peripheral/comparator/1.0/
echo 'cp periph.xml $IMPERAS_HOME/lib/Linux32/ImperasLib/mcgill.ca/peripheral/comparator/1.0/'
cp periph.xml $IMPERAS_HOME/lib/Linux32/ImperasLib/mcgill.ca/peripheral/comparator/1.0/