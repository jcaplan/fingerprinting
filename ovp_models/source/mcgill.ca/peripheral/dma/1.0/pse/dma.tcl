#
# Copyright (c) 2005-2014 Imperas Software Ltd. All Rights Reserved.
#
# THIS SOFTWARE CONTAINS CONFIDENTIAL INFORMATION AND TRADE SECRETS
# OF IMPERAS SOFTWARE LTD. USE, DISCLOSURE, OR REPRODUCTION IS PROHIBITED
# EXCEPT AS MAY BE PROVIDED FOR IN A WRITTEN AGREEMENT WITH IMPERAS SOFTWARE LTD.
#


imodelnewperipheral  \
    -name        dma \
    -vendor      mcgill.ca  \
    -library     peripheral \
    -version     1.0 \
    -constructor constructor \
    -destructor  destructor  \

iadddocumentation -name Description -text "DMA peripheral model"
    
# Master read and write ports
imodeladdbusmasterport -name "MREAD"   -addresswidth 32
iadddocumentation -name Description -text "DMA Registers Master Port - Read"
imodeladdbusmasterport -name "MWRITE"  -addresswidth 32
iadddocumentation -name Description -text "DMA Registers Master Port - Write"

# Slave port
imodeladdbusslaveport -name "DMACSP" -size 0x20 -mustbeconnected
iadddocumentation     -name Description -text "DMA Registers Slave Port"


imodeladdaddressblock -name ab8 -width 32  -offset 0 -size 0x20


imodeladdmmregister -name status 	   	-offset 0x00 -access rw -readfunction regRd -writefunction statusWr
imodeladdfield 		-name DONE 			-bitoffset 0 -width 1
imodeladdfield 		-name BUSY 			-bitoffset 1 -width 1
imodeladdfield 		-name REOP 			-bitoffset 2 -width 1
imodeladdfield 		-name WEOP 			-bitoffset 3 -width 1
imodeladdfield 		-name LEN 			-bitoffset 4 -width 1

imodeladdmmregister -name readaddress 	-offset 0x04 -access rw -readfunction regRd -writefunction regWr
imodeladdmmregister -name writeaddress 	-offset 0x08 -access rw -readfunction regRd -writefunction regWr
imodeladdmmregister -name length	 	-offset 0x0C -access rw -readfunction regRd -writefunction regWr

imodeladdmmregister -name control 	   	-offset 0x18 -access rw -readfunction regRd -writefunction controlWr
imodeladdfield 		-name BYTE 				-bitoffset 0  -width 1
imodeladdfield 		-name HW 				-bitoffset 1  -width 1
imodeladdfield 		-name WORD 				-bitoffset 2  -width 1
imodeladdfield 		-name GO 				-bitoffset 3  -width 1
imodeladdfield 		-name I_EN 				-bitoffset 4  -width 1
imodeladdfield 		-name REEN 				-bitoffset 5  -width 1
imodeladdfield 		-name WEEN 				-bitoffset 6  -width 1
imodeladdfield 		-name LEEN 				-bitoffset 7  -width 1
imodeladdfield 		-name RCON 				-bitoffset 8  -width 1
imodeladdfield 		-name WCON 				-bitoffset 9  -width 1
imodeladdfield 		-name DOUBLEWORD 		-bitoffset 10 -width 1
imodeladdfield 		-name QUADWORD 			-bitoffset 11 -width 1
imodeladdfield 		-name SOFTWARERESET 	-bitoffset 12 -width 1 


imodeladdnetport -name DMA_IRQ -type output