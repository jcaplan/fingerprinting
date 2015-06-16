#
# Copyright (c) 2005-2014 Imperas Software Ltd. All Rights Reserved.
#
# THIS SOFTWARE CONTAINS CONFIDENTIAL INFORMATION AND TRADE SECRETS
# OF IMPERAS SOFTWARE LTD. USE, DISCLOSURE, OR REPRODUCTION IS PROHIBITED
# EXCEPT AS MAY BE PROVIDED FOR IN A WRITTEN AGREEMENT WITH IMPERAS SOFTWARE LTD.
#

#
set desc "TLB."

set limitations "The range of the input slave port must not conflict with any exiting port connected to the bus.
The output bus width is hard coded to be 32 bits." 
#

imodelnewperipheral  \
    -imagefile   pse.pse  \
    -name        tlb \
    -vendor      mcgill.ca  \
    -library     peripheral \
    -version     1.0 \
    -constructor constructor

# Output bus size is 32 bits
set outputBusWidth  32

iadddocumentation -name Description -text $desc
iadddocumentation -name Licensing   -text "Open Source Apache 2.0"
iadddocumentation -name Limitations -text $limitations


imodeladdnetport -name "TLB_RESET" -type input -updatefunction do_reset
iadddocumentation     -name Description -text "reset signal"


# Define bus slave input port
imodeladdbusslaveport  -name TLB_SLAVE -remappable -mustbeconnected
imodeladdbusslaveport  -name TLB_CSR -size 0x400   
#-mustbeconnected

# Define bus master output port
imodeladdbusmasterport -name TLB_MASTER -addresswidth $outputBusWidth -mustbeconnected
imodeladdbusmasterport -name TLB_FPRINT_MASTER -addresswidth 32  

imodeladdaddressblock -name ab32 -port TLB_CSR    -width 32  -offset 0x0 -size 0x400
#imodeladdaddressblock -name ab32 -port TLB_SLAVE  -width 32  -offset 0x0 -size 0x08000000

imodeladdmmregister -name lineEnableReg     -offset 0x00 -access rw  -readfunction regRd32 -writefunction regWr32 
imodeladdmmregister -name lineValidReg      -offset 0x20 -access r   -readfunction regRd32 
imodeladdmmregister -name tlbActivateReg    -offset 0x200 -access rw -readfunction regRd32 -writefunction regWr32


imodeladdlocalmemory -addressblock TLB_CSR/ab32 -name physical_table -offset 0x180 -size 0x80 -writefunction memregWr
imodeladdlocalmemory -addressblock TLB_CSR/ab32 -name virtual_table -offset 0x100  -size 0x80 -writefunction memregWr

#imodeladdlocalmemory -addressblock TLB_SLAVE/ab32 -name space -offset 0x00  -size 0x08000000 -writefunction memWr -readfunction memRd


