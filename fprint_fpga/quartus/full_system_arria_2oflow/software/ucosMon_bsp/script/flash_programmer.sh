#!/bin/sh
#
# This file was automatically generated.
#
# It can be overwritten by nios2-flash-programmer-generate or nios2-flash-programmer-gui.
#

#
# Converting Binary File: /data/jcapla9/Imperas.20140430/ImperasLib/source/mcgill.ca/platform/archive.zip to: "../flash/archive_memory_0_generic_tristate_controller_0.flash"
#
bin2flash --input="/data/jcapla9/Imperas.20140430/ImperasLib/source/mcgill.ca/platform/archive.zip" --output="../flash/archive_memory_0_generic_tristate_controller_0.flash" --location=0x100000 

#
# Programming File: "../flash/archive_memory_0_generic_tristate_controller_0.flash" To Device: memory_0_generic_tristate_controller_0
#
nios2-flash-programmer "../flash/archive_memory_0_generic_tristate_controller_0.flash" --base=0x0 --sidp=0x1001020 --id=0x0 --timestamp=1415814601 --device=1 --instance=2 '--cable=USB-BlasterII on localhost [3-2]' --program 

