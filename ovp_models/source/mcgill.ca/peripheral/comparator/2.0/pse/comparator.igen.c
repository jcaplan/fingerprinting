
////////////////////////////////////////////////////////////////////////////////
//
//                W R I T T E N   B Y   I M P E R A S   I G E N
//
//                             Version 20150205.0
//                          Thu Jul 23 18:36:09 2015
//
////////////////////////////////////////////////////////////////////////////////


#include "comparator.igen.h"
/////////////////////////////// Port Declarations //////////////////////////////

COMP_CSR_SLAVE_ab32_dataT COMP_CSR_SLAVE_ab32_data;

COMP_FPRINT_SLAVE_ab32_dataT COMP_FPRINT_SLAVE_ab32_data;

handlesT handles;

/////////////////////////////// Diagnostic level ///////////////////////////////

// Test this variable to determine what diagnostics to output.
// eg. if (diagnosticLevel >= 1) bhmMessage("I", "comparator", "Example");
//     Predefined macros PSE_DIAG_LOW, PSE_DIAG_MEDIUM and PSE_DIAG_HIGH may be used
Uns32 diagnosticLevel;

/////////////////////////// Diagnostic level callback //////////////////////////

static void setDiagLevel(Uns32 new) {
    diagnosticLevel = new;
}

///////////////////////////// MMR Generic callbacks ////////////////////////////

static PPM_VIEW_CB(view32) {  *(Uns32*)data = *(Uns32*)user; }

//////////////////////////////// Bus Slave Ports ///////////////////////////////

static void installSlavePorts(void) {
    handles.COMP_CSR_SLAVE = ppmCreateSlaveBusPort("COMP_CSR_SLAVE", 4096);

    handles.COMP_FPRINT_SLAVE = ppmCreateSlaveBusPort("COMP_FPRINT_SLAVE", 4096);

}

//////////////////////////// Memory mapped registers ///////////////////////////

static void installRegisters(void) {

    ppmCreateRegister("ab32_exceptionReg",
        0,
        handles.COMP_CSR_SLAVE,
        0,
        4,
        csrRd32,
        exceptionRegWrite,
        view32,
        &(COMP_CSR_SLAVE_ab32_data.exceptionReg.value),
        True
    );
    ppmCreateRegister("ab32_statusReg",
        0,
        handles.COMP_CSR_SLAVE,
        4,
        4,
        csrRd32,
        0,
        view32,
        &(COMP_CSR_SLAVE_ab32_data.statusReg.value),
        True
    );
    ppmCreateRegister("ab32_successReg",
        0,
        handles.COMP_CSR_SLAVE,
        8,
        4,
        csrRd32,
        0,
        view32,
        &(COMP_CSR_SLAVE_ab32_data.successReg.value),
        True
    );
    ppmCreateRegister("ab32_failedReg",
        0,
        handles.COMP_CSR_SLAVE,
        12,
        4,
        csrRd32,
        0,
        view32,
        &(COMP_CSR_SLAVE_ab32_data.failedReg.value),
        True
    );
    ppmCreateRegister("ab32_catReg",
        0,
        handles.COMP_CSR_SLAVE,
        16,
        4,
        0,
        catRegWrite,
        view32,
        &(COMP_CSR_SLAVE_ab32_data.catReg.value),
        True
    );
    ppmCreateRegister("ab32_nmrReg",
        0,
        handles.COMP_CSR_SLAVE,
        20,
        4,
        0,
        nmrRegWrite,
        view32,
        &(COMP_CSR_SLAVE_ab32_data.nmrReg.value),
        True
    );
    ppmCreateRegister("ab32_successCounterReg",
        0,
        handles.COMP_CSR_SLAVE,
        24,
        4,
        0,
        successCounterRegWrite,
        view32,
        &(COMP_CSR_SLAVE_ab32_data.successCounterReg.value),
        True
    );

    ppmCreateRegister("ab32_currentState",
        0,
        handles.COMP_FPRINT_SLAVE,
        0,
        4,
        0,
        currentStateWrite,
        view32,
        &(COMP_FPRINT_SLAVE_ab32_data.currentState.value),
        True
    );
    ppmCreateRegister("ab32_fprint",
        0,
        handles.COMP_FPRINT_SLAVE,
        4,
        4,
        0,
        fprintWrite,
        view32,
        &(COMP_FPRINT_SLAVE_ab32_data.fprint.value),
        True
    );

}

/////////////////////////////// Bus Master Ports ///////////////////////////////

static void installMasterPorts(void) {
    handles.COMP_CHECKPOINT_IRQ_MASTER = ppmOpenAddressSpace("COMP_CHECKPOINT_IRQ_MASTER");
}

/////////////////////////////////// Net Ports //////////////////////////////////

static void installNetPorts(void) {
// To write to this net, use ppmWriteNet(handles.COMP_IRQ, value);

    handles.COMP_IRQ = ppmOpenNetPort("COMP_IRQ");

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

    ppmDocNodeP doc1_node = ppmDocAddSection(0, "Description");
    ppmDocAddText(doc1_node, "Comparator peripheral model");

    diagnosticLevel = 0;
    bhmInstallDiagCB(setDiagLevel);
    constructor();

    bhmWaitEvent(bhmGetSystemEvent(BHM_SE_END_OF_SIMULATION));
    destructor();
    return 0;
}

