
////////////////////////////////////////////////////////////////////////////////
//
//                W R I T T E N   B Y   I M P E R A S   I G E N
//
//                             Version 20150205.0
//                          Tue Jun 23 14:43:06 2015
//
////////////////////////////////////////////////////////////////////////////////


#include "reset_monitor.igen.h"
/////////////////////////////// Port Declarations //////////////////////////////

RESET_MONITOR_SLAVE_ab32_dataT RESET_MONITOR_SLAVE_ab32_data;

handlesT handles;

/////////////////////////////// Diagnostic level ///////////////////////////////

// Test this variable to determine what diagnostics to output.
// eg. if (diagnosticLevel >= 1) bhmMessage("I", "reset_monitor", "Example");
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
    handles.RESET_MONITOR_SLAVE = ppmCreateSlaveBusPort("RESET_MONITOR_SLAVE", 1024);
    if (!handles.RESET_MONITOR_SLAVE) {
        bhmMessage("E", "PPM_SPNC", "Could not connect port 'RESET_MONITOR_SLAVE'");
    }

}

//////////////////////////// Memory mapped registers ///////////////////////////

static void installRegisters(void) {

    ppmCreateRegister("ab32_enableReg",
        0,
        handles.RESET_MONITOR_SLAVE,
        0,
        4,
        regRd32,
        writeEnableReg,
        view32,
        &(RESET_MONITOR_SLAVE_ab32_data.enableReg.value),
        True
    );
    ppmCreateRegister("ab32_monitorReg",
        0,
        handles.RESET_MONITOR_SLAVE,
        4,
        4,
        regRd32,
        writeMonitorReg,
        view32,
        &(RESET_MONITOR_SLAVE_ab32_data.monitorReg.value),
        True
    );
    ppmCreateRegister("ab32_coreStatusReg",
        0,
        handles.RESET_MONITOR_SLAVE,
        8,
        4,
        regRd32,
        writeCoreReg,
        view32,
        &(RESET_MONITOR_SLAVE_ab32_data.coreStatusReg.value),
        True
    );

}

/////////////////////////////////// Net Ports //////////////////////////////////

static void installNetPorts(void) {
// To write to this net, use ppmWriteNet(handles.cpum_notify, value);

    handles.cpum_notify = ppmOpenNetPort("cpum_notify");

}

////////////////////////////////// Constructor /////////////////////////////////

PPM_CONSTRUCTOR_CB(periphConstructor) {
    installSlavePorts();
    installRegisters();
    installNetPorts();
}

///////////////////////////////////// Main /////////////////////////////////////

int main(int argc, char *argv[]) {

    ppmDocNodeP doc1_node = ppmDocAddSection(0, "Description");
    ppmDocAddText(doc1_node, "reset monitor peripheral model");

    diagnosticLevel = 0;
    bhmInstallDiagCB(setDiagLevel);
    constructor();

    bhmWaitEvent(bhmGetSystemEvent(BHM_SE_END_OF_SIMULATION));
    destructor();
    return 0;
}

