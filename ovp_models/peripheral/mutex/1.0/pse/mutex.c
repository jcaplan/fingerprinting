
////////////////////////////////////////////////////////////////////////////////
//
//                W R I T T E N   B Y   I M P E R A S   I G E N
//
//                             Version 20140430.0
//                          Mon Aug 18 14:51:00 2014
//
////////////////////////////////////////////////////////////////////////////////

#include "mutex.igen.h"
//////////////////////////////// Callback stubs ////////////////////////////////

PPM_REG_READ_CB(regRd32) {
    // YOUR CODE HERE (regRd32)
    return *(Uns32*)user;
}

PPM_REG_WRITE_CB(regWr32) {
    // YOUR CODE HERE (regWr32)
    *(Uns32*)user = data;
}

PPM_CONSTRUCTOR_CB(constructor) {
    // YOUR CODE HERE (pre constructor)
    periphConstructor();
    // YOUR CODE HERE (post constructor)
    Uns32 v, owner;
    bhmIntegerAttribute("InitialValue", &v); 
    bhmIntegerAttribute("InitialOwner", &owner);

    Uns32 mutex = (owner << 16) | v;
    MUTEX_SLAVE_ab32_data.mutex.value = mutex;

}

PPM_DESTRUCTOR_CB(destructor) {
    // YOUR CODE HERE (destructor)
}

