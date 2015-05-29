
////////////////////////////////////////////////////////////////////////////////
//
//                W R I T T E N   B Y   I M P E R A S   I G E N
//
//                             Version 20140430.0
//                          Mon Aug 18 15:38:43 2014
//
////////////////////////////////////////////////////////////////////////////////

#include "cpu_irq.igen.h"
//////////////////////////////// Callback stubs ////////////////////////////////

PPM_REG_READ_CB(regRd32) {
    // YOUR CODE HERE (regRd32)
    return *(Uns32*)user;
}

PPM_REG_WRITE_CB(regWr32) {
    // YOUR CODE HERE (regWr32)
    *(Uns32*)user = data;
    if((Uns32*)user == &(CPU_IRQ_SLAVE_ab32_data.cpum.value))
    	ppmWriteNet(handles.cpum_irq, data);
    else if((Uns32*)user == &(CPU_IRQ_SLAVE_ab32_data.cpu0.value))
    	ppmWriteNet(handles.cpu0_irq, data);
 	else if((Uns32*)user == &(CPU_IRQ_SLAVE_ab32_data.cpu1.value))
    	ppmWriteNet(handles.cpu1_irq, data);
    else if((Uns32*)user == &(CPU_IRQ_SLAVE_ab32_data.cpu2.value))
        ppmWriteNet(handles.cpu2_irq, data);
    else if((Uns32*)user == &(CPU_IRQ_SLAVE_ab32_data.cpu3.value))
        ppmWriteNet(handles.cpu3_irq, data);
    else if((Uns32*)user == &(CPU_IRQ_SLAVE_ab32_data.cpu4.value))
        ppmWriteNet(handles.cpu4_irq, data);
    else if((Uns32*)user == &(CPU_IRQ_SLAVE_ab32_data.cpu5.value))
        ppmWriteNet(handles.cpu5_irq, data);
    else if((Uns32*)user == &(CPU_IRQ_SLAVE_ab32_data.cpu6.value))
        ppmWriteNet(handles.cpu6_irq, data);
    else if((Uns32*)user == &(CPU_IRQ_SLAVE_ab32_data.cpu7.value))
        ppmWriteNet(handles.cpu7_irq, data);
    
       
}

PPM_CONSTRUCTOR_CB(constructor) {
    // YOUR CODE HERE (pre constructor)
    periphConstructor();
    
    // YOUR CODE HERE (post constructor)
}

PPM_DESTRUCTOR_CB(destructor) {
    // YOUR CODE HERE (destructor)
}

