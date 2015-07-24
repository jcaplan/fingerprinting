
////////////////////////////////////////////////////////////////////////////////
//
//                W R I T T E N   B Y   I M P E R A S   I G E N
//
//                             Version 20140430.0
//                          Tue Jul 29 17:25:54 2014
//
////////////////////////////////////////////////////////////////////////////////

#include "fprint.igen.h"
#include "peripheral/impTypes.h"
#include "peripheral/ppm.h"
#include "peripheral/bhm.h"
#include "crc.h"
#include "fprint.igen.h"

#define COMPARATOR_BASE_ADDRESS 0x1000000
//////////////////////////////// Callback stubs ////////////////////////////////

Uns32 fprint[8];
Uns32 fprint_old;
Uns32 count[8];
Uns32 count_old;
Uns32 pauseReg;
Uns32 currentState[8];
Uns32 pause_index;
Uns32 coreID;
Uns32 mode = 0;

Uns32 old_address = 0xcafebabe;
Uns32 old_data    = 0xcafebabe;



Uns32 translation_table[33];


#define FPRINT_VALID 1
#define FPRINT_EMPTY 0


Uns32 fprint_pause_hold[2] = {FPRINT_EMPTY, 0};




PPM_NET_CB(do_reset){


}

PPM_NET_CB(do_store) {

	
	  //We reach here because of a net


}

PPM_REG_READ_CB(pause) {

	return *(Uns32*)user;
}

PPM_REG_READ_CB(regRd32) {
	// YOUR CODE HERE (regRd32)
	return 0xdeadbeef;
}

PPM_REG_WRITE_CB(regWr32) {
	// YOUR CODE HERE (regWr32)

}

PPM_REG_READ_CB(unpause) {
	 
	return *(Uns32*)user;
	
}

PPM_WRITE_CB(memregWr){
  
 
}

PPM_CONSTRUCTOR_CB(constructor) {
	// YOUR CODE HERE (pre constructor)
	periphConstructor();
	// YOUR CODE HERE (post constructor)
	crcInit();
	FPRINT_CSR_SLAVE_ab32_data.counterMax.value = 0xfff;
	bhmIntegerAttribute("coreID", &coreID);       
	pause_index = 0;
}

PPM_DESTRUCTOR_CB(destructor) {
	// YOUR CODE HERE (destructor)
}

