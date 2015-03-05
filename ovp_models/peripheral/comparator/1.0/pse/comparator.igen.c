
////////////////////////////////////////////////////////////////////////////////
//
//                W R I T T E N   B Y   I M P E R A S   I G E N
//
//                             Version 20140430.0
//                          Fri Oct 31 11:43:01 2014
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////// Description /////////////////////////////////

// Comparator peripheral model


#include "comparator.igen.h"
/////////////////////////////// Port Declarations //////////////////////////////

COMP_CSR_SLAVE_ab32_dataT COMP_CSR_SLAVE_ab32_data;

COMP_FPRINT_SLAVE_ab32_dataT COMP_FPRINT_SLAVE_ab32_data;

handlesT handles;

/////////////////////////////// Diagnostic level ///////////////////////////////

// Test this variable to determine what diagnostics to output.
// eg. if (diagnosticLevel > 0) bhmMessage("I", "comparator", "Example");

Uns32 diagnosticLevel;

/////////////////////////// Diagnostic level callback //////////////////////////

static void setDiagLevel(Uns32 new) {
    diagnosticLevel = new;
}

//////////////////////////////// Bus Slave Ports ///////////////////////////////

static void installSlavePorts(void) {
    handles.COMP_CSR_SLAVE = ppmCreateSlaveBusPort("COMP_CSR_SLAVE", 4096);

    ppmInstallReadCallback(memRd, (void*)0x0, handles.COMP_CSR_SLAVE+0x0, 0x1000);
    ppmInstallWriteCallback(memWr, (void*)0x0, handles.COMP_CSR_SLAVE+0x0, 0x1000);
    handles.COMP_FPRINT_SLAVE = ppmCreateSlaveBusPort("COMP_FPRINT_SLAVE", 4096);

    ppmInstallWriteCallback(fprintWr, (void*)0x0, handles.COMP_FPRINT_SLAVE+0x0, 0x1000);
}

/////////////////////////////////// Net Ports //////////////////////////////////

static void installNetPorts(void) {
// To write to this net, use ppmWriteNet(handles.COMP_IRQ, value);

    handles.COMP_IRQ = ppmOpenNetPort("COMP_IRQ");

}

////////////////////////////////// Constructor /////////////////////////////////

PPM_CONSTRUCTOR_CB(periphConstructor) {
    installSlavePorts();
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

