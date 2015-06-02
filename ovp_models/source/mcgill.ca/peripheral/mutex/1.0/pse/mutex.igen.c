
////////////////////////////////////////////////////////////////////////////////
//
//                W R I T T E N   B Y   I M P E R A S   I G E N
//
//                             Version 20140430.0
//                          Mon Aug 18 14:59:50 2014
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////// Description /////////////////////////////////

// Mutex peripheral model


#include "mutex.igen.h"
/////////////////////////////// Port Declarations //////////////////////////////

MUTEX_SLAVE_ab32_dataT MUTEX_SLAVE_ab32_data;

handlesT handles;

/////////////////////////////// Diagnostic level ///////////////////////////////

// Test this variable to determine what diagnostics to output.
// eg. if (diagnosticLevel > 0) bhmMessage("I", "mutex", "Example");

Uns32 diagnosticLevel;

/////////////////////////// Diagnostic level callback //////////////////////////

static void setDiagLevel(Uns32 new) {
    diagnosticLevel = new;
}

///////////////////////////// MMR Generic callbacks ////////////////////////////

static PPM_VIEW_CB(view32) {  *(Uns32*)data = *(Uns32*)user; }

//////////////////////////////// Bus Slave Ports ///////////////////////////////

static void installSlavePorts(void) {
    handles.MUTEX_SLAVE = ppmCreateSlaveBusPort("MUTEX_SLAVE", 8);
    if (!handles.MUTEX_SLAVE) {
        bhmMessage("E", "PPM_SPNC", "Could not connect port 'MUTEX_SLAVE'");
    }

}

//////////////////////////// Memory mapped registers ///////////////////////////

static void installRegisters(void) {

    ppmCreateRegister("ab32_mutex",
        0,
        handles.MUTEX_SLAVE,
        0,
        4,
        regRd32,
        regWr32,
        view32,
        &(MUTEX_SLAVE_ab32_data.mutex.value),
        True
    );
    ppmCreateRegister("ab32_reset",
        0,
        handles.MUTEX_SLAVE,
        4,
        4,
        regRd32,
        regWr32,
        view32,
        &(MUTEX_SLAVE_ab32_data.reset.value),
        True
    );

}

////////////////////////////////// Constructor /////////////////////////////////

PPM_CONSTRUCTOR_CB(periphConstructor) {
    installSlavePorts();
    installRegisters();
}

///////////////////////////////////// Main /////////////////////////////////////

int main(int argc, char *argv[]) {
    diagnosticLevel = 0;
    bhmInstallDiagCB(setDiagLevel);
    constructor();

    bhmWaitEvent(bhmGetSystemEvent(BHM_SE_END_OF_SIMULATION));
    destructor();
    return 0;
}

