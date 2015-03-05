#!/bin/bash

igen.exe --batch fprint.tcl --writec fprint --writexml periph.xml
make NOVLNV=1
echo 'cp pse.pse $IMPERAS_HOME/lib/Linux32/ImperasLib/mcgill.ca/peripheral/fprint/1.0/'
cp pse.pse $IMPERAS_HOME/lib/Linux32/ImperasLib/mcgill.ca/peripheral/fprint/1.0/
echo 'cp periph.xml $IMPERAS_HOME/lib/Linux32/ImperasLib/mcgill.ca/peripheral/fprint/1.0/'
cp periph.xml $IMPERAS_HOME/lib/Linux32/ImperasLib/mcgill.ca/peripheral/fprint/1.0/