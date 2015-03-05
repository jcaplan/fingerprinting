
////////////////////////////////////////////////////////////////////////////////
//
//                W R I T T E N   B Y   I M P E R A S   I G E N
//
//                             Version 20140430.0
//                          Fri Oct  3 15:35:36 2014
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////// Description /////////////////////////////////

// DMA peripheral model


#include "dma.igen.h"
/////////////////////////////// Port Declarations //////////////////////////////

DMACSP_ab8_dataT DMACSP_ab8_data;

handlesT handles;

/////////////////////////////// Diagnostic level ///////////////////////////////

// Test this variable to determine what diagnostics to output.
// eg. if (diagnosticLevel > 0) bhmMessage("I", "dma", "Example");

Uns32 diagnosticLevel;

/////////////////////////// Diagnostic level callback //////////////////////////

static void setDiagLevel(Uns32 new) {
    diagnosticLevel = new;
}

///////////////////////////// MMR Generic callbacks ////////////////////////////

static PPM_VIEW_CB(view32) {  *(Uns32*)data = *(Uns32*)user; }

//////////////////////////////// Bus Slave Ports ///////////////////////////////

static void installSlavePorts(void) {
    handles.DMACSP = ppmCreateSlaveBusPort("DMACSP", 32);
    if (!handles.DMACSP) {
        bhmMessage("E", "PPM_SPNC", "Could not connect port 'DMACSP'");
    }

}

//////////////////////////// Memory mapped registers ///////////////////////////

static void installRegisters(void) {

    ppmCreateRegister("ab8_status",
        0,
        handles.DMACSP,
        0,
        4,
        regRd,
        statusWr,
        view32,
        &(DMACSP_ab8_data.status.value),
        True
    );
    ppmCreateRegister("ab8_readaddress",
        0,
        handles.DMACSP,
        4,
        4,
        regRd,
        regWr,
        view32,
        &(DMACSP_ab8_data.readaddress.value),
        True
    );
    ppmCreateRegister("ab8_writeaddress",
        0,
        handles.DMACSP,
        8,
        4,
        regRd,
        regWr,
        view32,
        &(DMACSP_ab8_data.writeaddress.value),
        True
    );
    ppmCreateRegister("ab8_length",
        0,
        handles.DMACSP,
        12,
        4,
        regRd,
        regWr,
        view32,
        &(DMACSP_ab8_data.length.value),
        True
    );
    ppmCreateRegister("ab8_control",
        0,
        handles.DMACSP,
        24,
        4,
        regRd,
        controlWr,
        view32,
        &(DMACSP_ab8_data.control.value),
        True
    );

}

/////////////////////////////// Bus Master Ports ///////////////////////////////

static void installMasterPorts(void) {
    handles.MREAD = ppmOpenAddressSpace("MREAD");
    handles.MWRITE = ppmOpenAddressSpace("MWRITE");
}

/////////////////////////////////// Net Ports //////////////////////////////////

static void installNetPorts(void) {
// To write to this net, use ppmWriteNet(handles.DMA_IRQ, value);

    handles.DMA_IRQ = ppmOpenNetPort("DMA_IRQ");

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

