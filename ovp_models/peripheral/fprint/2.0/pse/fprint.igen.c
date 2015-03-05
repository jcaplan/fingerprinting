
////////////////////////////////////////////////////////////////////////////////
//
//                W R I T T E N   B Y   I M P E R A S   I G E N
//
//                             Version 20140430.0
//                          Thu Oct  9 12:18:30 2014
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////// Description /////////////////////////////////

// Fingerprint peripheral model


#include "fprint.igen.h"
/////////////////////////////// Port Declarations //////////////////////////////

FPRINT_CSR_SLAVE_ab32_dataT FPRINT_CSR_SLAVE_ab32_data;

FPRINT_TLB_SLAVE_ab32_dataT FPRINT_TLB_SLAVE_ab32_data;

handlesT handles;

/////////////////////////////// Diagnostic level ///////////////////////////////

// Test this variable to determine what diagnostics to output.
// eg. if (diagnosticLevel > 0) bhmMessage("I", "fprint", "Example");

Uns32 diagnosticLevel;

/////////////////////////// Diagnostic level callback //////////////////////////

static void setDiagLevel(Uns32 new) {
    diagnosticLevel = new;
}

///////////////////////////// MMR Generic callbacks ////////////////////////////

static PPM_VIEW_CB(view32) {  *(Uns32*)data = *(Uns32*)user; }

//////////////////////////////// Bus Slave Ports ///////////////////////////////

static void installSlavePorts(void) {
    handles.FPRINT_CSR_SLAVE = ppmCreateSlaveBusPort("FPRINT_CSR_SLAVE", 320);
    if (!handles.FPRINT_CSR_SLAVE) {
        bhmMessage("E", "PPM_SPNC", "Could not connect port 'FPRINT_CSR_SLAVE'");
    }

    handles.FPRINT_TLB_SLAVE = ppmCreateSlaveBusPort("FPRINT_TLB_SLAVE", 1024);

    ppmInstallWriteCallback(memregWr, (void*)0x0, handles.FPRINT_TLB_SLAVE+0x0, 0x84);
}

//////////////////////////// Memory mapped registers ///////////////////////////

static void installRegisters(void) {

    ppmCreateRegister("ab32_currentState",
        "current state register",
        handles.FPRINT_CSR_SLAVE,
        0,
        4,
        regRd32,
        regWr32,
        view32,
        &(FPRINT_CSR_SLAVE_ab32_data.currentState.value),
        True
    );
    ppmCreateRegister("ab32_pauseReg",
        0,
        handles.FPRINT_CSR_SLAVE,
        4,
        4,
        regRd32,
        regWr32,
        view32,
        &(FPRINT_CSR_SLAVE_ab32_data.pauseReg.value),
        True
    );
    ppmCreateRegister("ab32_pauseStrobe",
        "pause strobe",
        handles.FPRINT_CSR_SLAVE,
        8,
        4,
        pause,
        0,
        view32,
        &(FPRINT_CSR_SLAVE_ab32_data.pauseStrobe.value),
        True
    );
    ppmCreateRegister("ab32_unpauseStrobe",
        "unpause strobe",
        handles.FPRINT_CSR_SLAVE,
        12,
        4,
        unpause,
        0,
        view32,
        &(FPRINT_CSR_SLAVE_ab32_data.unpauseStrobe.value),
        True
    );
    ppmCreateRegister("ab32_counterMax",
        "counter max",
        handles.FPRINT_CSR_SLAVE,
        16,
        4,
        regRd32,
        regWr32,
        view32,
        &(FPRINT_CSR_SLAVE_ab32_data.counterMax.value),
        True
    );


}

/////////////////////////////// Bus Master Ports ///////////////////////////////

static void installMasterPorts(void) {
    handles.FPRINT_MASTER = ppmOpenAddressSpace("FPRINT_MASTER");
}

/////////////////////////////////// Net Ports //////////////////////////////////

static void installNetPorts(void) {
    handles.FPRINT_WRITE = ppmOpenNetPort("FPRINT_WRITE");
    if (handles.FPRINT_WRITE) {
        ppmInstallNetCallback(handles.FPRINT_WRITE, do_store, (void*)0);
    }

    handles.FPRINT_WRITE_ADDRESS = ppmOpenNetPort("FPRINT_WRITE_ADDRESS");

    handles.FPRINT_WRITE_DATA = ppmOpenNetPort("FPRINT_WRITE_DATA");

}

////////////////////////////////// Constructor /////////////////////////////////

PPM_CONSTRUCTOR_CB(periphConstructor) {
    installSlavePorts();
    installRegisters();
    installMasterPorts();
    installNetPorts();
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

