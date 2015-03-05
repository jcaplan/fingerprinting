#!/bin/sh
#
# This file was automatically generated.
#
# It can be overwritten by nios2-flash-programmer-generate or nios2-flash-programmer-gui.
#

#
# Converting Binary File: C:\Users\Jonah\Dropbox\Fingerprinting\fprint_ptype_1_1\quartus\full_system_arria\software\input_small.zip to: "..\flash/input_small_generic_tristate_controller_0.flash"
#
bin2flash --input="C:/Users/Jonah/Dropbox/Fingerprinting/fprint_ptype_1_1/quartus/full_system_arria/software/input_small.zip" --output="../flash/input_small_generic_tristate_controller_0.flash" --location=0x0 --verbose 

#
# Programming File: "..\flash/input_small_generic_tristate_controller_0.flash" To Device: generic_tristate_controller_0
#
nios2-flash-programmer "../flash/input_small_generic_tristate_controller_0.flash" --base=0x400000 --sidp=0x1001020 --id=0x0 --accept-bad-sysid --device=1 --instance=0 '--cable=USB-BlasterII on localhost [USB-1]' --program --verbose 

