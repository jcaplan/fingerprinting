
////////////////////////////////////////////////////////////////////////////////
//
//                W R I T T E N   B Y   I M P E R A S   I G E N
//
//                             Version 20140430.0
//                          Tue Oct  7 11:22:25 2014
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////// Description /////////////////////////////////

// TLB.

/////////////////////////////////// Licensing //////////////////////////////////

// Open Source Apache 2.0

////////////////////////////////// Limitations /////////////////////////////////

// The range of the input slave port must not conflict with any exiting port conn
// ected to the bus.
// The output bus width is hard coded to be 32 bits.


#include "tlb.igen.h"
/////////////////////////////// Port Declarations //////////////////////////////

TLB_CSR_ab32_dataT TLB_CSR_ab32_data;

handlesT handles;

/////////////////////////////// Diagnostic level ///////////////////////////////

// Test this variable to determine what diagnostics to output.
// eg. if (diagnosticLevel > 0) bhmMessage("I", "tlb", "Example");

Uns32 diagnosticLevel;

/////////////////////////// Diagnostic level callback //////////////////////////

static void setDiagLevel(Uns32 new) {
    diagnosticLevel = new;
}

///////////////////////////// MMR Generic callbacks ////////////////////////////

static PPM_VIEW_CB(view32) {  *(Uns32*)data = *(Uns32*)user; }

//////////////////////////////// Bus Slave Ports ///////////////////////////////

static void installSlavePorts(void) {
    handles.TLB_SLAVE = ppmCreateSlaveBusPort("TLB_SLAVE", 134217727);

    handles.TLB_CSR = ppmCreateSlaveBusPort("TLB_CSR", 1024);

    ppmInstallWriteCallback(memregWr, (void*)0x0, handles.TLB_CSR+0x180, 0x80);
    ppmInstallWriteCallback(memregWr, (void*)0x0, handles.TLB_CSR+0x100, 0x80);
}

//////////////////////////// Memory mapped registers ///////////////////////////

static void installRegisters(void) {


    ppmCreateRegister("ab32_lineEnableReg",
        0,
        handles.TLB_CSR,
        0,
        4,
        regRd32,
        regWr32,
        view32,
        &(TLB_CSR_ab32_data.lineEnableReg.value),
        True
    );
    ppmCreateRegister("ab32_lineValidReg",
        0,
        handles.TLB_CSR,
        32,
        4,
        regRd32,
        0,
        view32,
        &(TLB_CSR_ab32_data.lineValidReg.value),
        True
    );
    ppmCreateRegister("ab32_tlbActivateReg",
        0,
        handles.TLB_CSR,
        512,
        4,
        regRd32,
        regWr32,
        view32,
        &(TLB_CSR_ab32_data.tlbActivateReg.value),
        True
    );

}

/////////////////////////////// Bus Master Ports ///////////////////////////////

static void installMasterPorts(void) {
    handles.TLB_MASTER = ppmOpenAddressSpace("TLB_MASTER");
    if (!handles.TLB_MASTER) {
        bhmMessage("E", "PPM_MPNC", "Could not connect port 'TLB_MASTER'");
    }
    handles.TLB_FPRINT_MASTER = ppmOpenAddressSpace("TLB_FPRINT_MASTER");
}

////////////////////////////////// Constructor /////////////////////////////////

PPM_CONSTRUCTOR_CB(periphConstructor) {
    installSlavePorts();
    installRegisters();
    installMasterPorts();
}

///////////////////////////////////// Main /////////////////////////////////////

int main(int argc, char *argv[]) {
    diagnosticLevel = 0;
    bhmInstallDiagCB(setDiagLevel);
    constructor();

    return 0;
}

