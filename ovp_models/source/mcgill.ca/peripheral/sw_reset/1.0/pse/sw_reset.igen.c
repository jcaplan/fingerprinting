
////////////////////////////////////////////////////////////////////////////////
//
//                W R I T T E N   B Y   I M P E R A S   I G E N
//
//                             Version 20150205.0
//                          Mon Jun  8 15:20:35 2015
//
////////////////////////////////////////////////////////////////////////////////


#include "sw_reset.igen.h"
/////////////////////////////// Port Declarations //////////////////////////////

SW_RESET_SLAVE_ab32_dataT SW_RESET_SLAVE_ab32_data;

handlesT handles;

/////////////////////////////// Diagnostic level ///////////////////////////////

// Test this variable to determine what diagnostics to output.
// eg. if (diagnosticLevel >= 1) bhmMessage("I", "sw_reset", "Example");
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
    handles.SW_RESET_SLAVE = ppmCreateSlaveBusPort("SW_RESET_SLAVE", 9216);
    if (!handles.SW_RESET_SLAVE) {
        bhmMessage("E", "PPM_SPNC", "Could not connect port 'SW_RESET_SLAVE'");
    }

}

//////////////////////////// Memory mapped registers ///////////////////////////

static void installRegisters(void) {

    ppmCreateRegister("ab32_cpum",
        0,
        handles.SW_RESET_SLAVE,
        2048,
        4,
        regRd32,
        regWr32,
        view32,
        &(SW_RESET_SLAVE_ab32_data.cpum.value),
        True
    );
    ppmCreateRegister("ab32_cpu0",
        0,
        handles.SW_RESET_SLAVE,
        0,
        4,
        regRd32,
        regWr32,
        view32,
        &(SW_RESET_SLAVE_ab32_data.cpu0.value),
        True
    );
    ppmCreateRegister("ab32_cpu1",
        0,
        handles.SW_RESET_SLAVE,
        1024,
        4,
        regRd32,
        regWr32,
        view32,
        &(SW_RESET_SLAVE_ab32_data.cpu1.value),
        True
    );
    ppmCreateRegister("ab32_cpu2",
        0,
        handles.SW_RESET_SLAVE,
        3072,
        4,
        regRd32,
        regWr32,
        view32,
        &(SW_RESET_SLAVE_ab32_data.cpu2.value),
        True
    );
    ppmCreateRegister("ab32_cpu3",
        0,
        handles.SW_RESET_SLAVE,
        4096,
        4,
        regRd32,
        regWr32,
        view32,
        &(SW_RESET_SLAVE_ab32_data.cpu3.value),
        True
    );
    ppmCreateRegister("ab32_cpu4",
        0,
        handles.SW_RESET_SLAVE,
        5120,
        4,
        regRd32,
        regWr32,
        view32,
        &(SW_RESET_SLAVE_ab32_data.cpu4.value),
        True
    );
    ppmCreateRegister("ab32_cpu5",
        0,
        handles.SW_RESET_SLAVE,
        6144,
        4,
        regRd32,
        regWr32,
        view32,
        &(SW_RESET_SLAVE_ab32_data.cpu5.value),
        True
    );
    ppmCreateRegister("ab32_cpu6",
        0,
        handles.SW_RESET_SLAVE,
        7168,
        4,
        regRd32,
        regWr32,
        view32,
        &(SW_RESET_SLAVE_ab32_data.cpu6.value),
        True
    );
    ppmCreateRegister("ab32_cpu7",
        0,
        handles.SW_RESET_SLAVE,
        8192,
        4,
        regRd32,
        regWr32,
        view32,
        &(SW_RESET_SLAVE_ab32_data.cpu7.value),
        True
    );

}

/////////////////////////////////// Net Ports //////////////////////////////////

static void installNetPorts(void) {
// To write to this net, use ppmWriteNet(handles.cpum_reset, value);

    handles.cpum_reset = ppmOpenNetPort("cpum_reset");

// To write to this net, use ppmWriteNet(handles.cpu0_reset, value);

    handles.cpu0_reset = ppmOpenNetPort("cpu0_reset");

// To write to this net, use ppmWriteNet(handles.cpu1_reset, value);

    handles.cpu1_reset = ppmOpenNetPort("cpu1_reset");

// To write to this net, use ppmWriteNet(handles.cpu2_reset, value);

    handles.cpu2_reset = ppmOpenNetPort("cpu2_reset");

// To write to this net, use ppmWriteNet(handles.cpu3_reset, value);

    handles.cpu3_reset = ppmOpenNetPort("cpu3_reset");

// To write to this net, use ppmWriteNet(handles.cpu4_reset, value);

    handles.cpu4_reset = ppmOpenNetPort("cpu4_reset");

// To write to this net, use ppmWriteNet(handles.cpu5_reset, value);

    handles.cpu5_reset = ppmOpenNetPort("cpu5_reset");

// To write to this net, use ppmWriteNet(handles.cpu6_reset, value);

    handles.cpu6_reset = ppmOpenNetPort("cpu6_reset");

// To write to this net, use ppmWriteNet(handles.cpu7_reset, value);

    handles.cpu7_reset = ppmOpenNetPort("cpu7_reset");

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
    ppmDocAddText(doc1_node, "SW Reset peripheral model");

    diagnosticLevel = 0;
    bhmInstallDiagCB(setDiagLevel);
    constructor();

    bhmWaitEvent(bhmGetSystemEvent(BHM_SE_END_OF_SIMULATION));
    destructor();
    return 0;
}

