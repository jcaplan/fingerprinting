imodelnewperipheral  \
    -name        comparator \
    -vendor      mcgill.ca  \
    -library     peripheral \
    -version     1.0 \
    -constructor constructor \
    -destructor  destructor

iadddocumentation -name Description -text "Comparator peripheral model"


#fingerprinting csr slave port
imodeladdbusslaveport -name COMP_CSR_SLAVE -size 0x1000 
#-mustbeconnected
iadddocumentation     -name Description -text "Comparator control register slave port"

#fingerprinting csr slave port
imodeladdbusslaveport -name COMP_FPRINT_SLAVE -size 0x1000 
#-mustbeconnected
iadddocumentation     -name Description -text "Comparator fprint slave port"

imodeladdaddressblock -name ab32 -port COMP_FPRINT_SLAVE  -width 32  -offset 0x0 -size 0x1000

#Interrupt net
imodeladdnetport -name COMP_IRQ -type output
iadddocumentation     -name Description -text "comparator interrupt"


imodeladdaddressblock -name ab32 -port COMP_CSR_SLAVE  -width 32  -offset 0x0 -size 0x200


#start and end registers

imodeladdlocalmemory -addressblock COMP_CSR_SLAVE/ab32 -name csr -offset 0x0  -size 0x1000 -writefunction memWr -readfunction memRd


imodeladdlocalmemory -addressblock COMP_FPRINT_SLAVE/ab32 -name fprints   -offset 0x0 -size 0x1000 -writefunction fprintWr