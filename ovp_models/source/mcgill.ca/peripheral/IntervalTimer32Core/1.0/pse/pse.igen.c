
////////////////////////////////////////////////////////////////////////////////
//
//                W R I T T E N   B Y   I M P E R A S   I G E N
//
//                             Version 20150205.0
//                          Thu Jul 23 18:45:28 2015
//
////////////////////////////////////////////////////////////////////////////////


#include "pse.igen.h"
/////////////////////////////// Port Declarations //////////////////////////////

sp1_reg0_dataT sp1_reg0_data;

handlesT handles;

/////////////////////////////// Diagnostic level ///////////////////////////////

// Test this variable to determine what diagnostics to output.
// eg. if (diagnosticLevel >= 1) bhmMessage("I", "IntervalTimer32Core", "Example");
//     Predefined macros PSE_DIAG_LOW, PSE_DIAG_MEDIUM and PSE_DIAG_HIGH may be used
Uns32 diagnosticLevel;

/////////////////////////// Diagnostic level callback //////////////////////////

static void setDiagLevel(Uns32 new) {
    diagnosticLevel = new;
}

///////////////////////////// MMR Generic callbacks ////////////////////////////

static PPM_VIEW_CB(view16) {  *(Uns16*)data = *(Uns16*)user; }

//////////////////////////////// View callbacks ////////////////////////////////

static PPM_VIEW_CB(view_sp1_reg0_periodl) {
    *(Uns16*)data = sp1_reg0_data.periodl.value;
}

static PPM_VIEW_CB(view_sp1_reg0_periodh) {
    *(Uns16*)data = sp1_reg0_data.periodh.value;
}

static PPM_VIEW_CB(view_sp1_reg0_snapl) {
    *(Uns16*)data = sp1_reg0_data.snapl.value;
}

static PPM_VIEW_CB(view_sp1_reg0_snaph) {
    *(Uns16*)data = sp1_reg0_data.snaph.value;
}
//////////////////////////////// Bus Slave Ports ///////////////////////////////

static void installSlavePorts(void) {
    handles.sp1 = ppmCreateSlaveBusPort("sp1", 24);

}

//////////////////////////// Memory mapped registers ///////////////////////////

static void installRegisters(void) {

    ppmCreateRegister("reg0_status",
        0,
        handles.sp1,
        0,
        2,
        read_status,
        write_status,
        view16,
        &(sp1_reg0_data.status.value),
        True
    );
    ppmCreateRegister("reg0_control",
        0,
        handles.sp1,
        4,
        2,
        read_control,
        write_control,
        view16,
        &(sp1_reg0_data.control.value),
        True
    );
    ppmCreateRegister("reg0_periodl",
        0,
        handles.sp1,
        8,
        2,
        read_period,
        write_period,
        view_sp1_reg0_periodl,
        (void*)0x0,
        True
    );
    ppmCreateRegister("reg0_periodh",
        0,
        handles.sp1,
        12,
        2,
        read_period,
        write_period,
        view_sp1_reg0_periodh,
        (void*)0x1,
        True
    );
    ppmCreateRegister("reg0_snapl",
        0,
        handles.sp1,
        16,
        2,
        read_snap,
        write_snap,
        view_sp1_reg0_snapl,
        (void*)0x0,
        True
    );
    ppmCreateRegister("reg0_snaph",
        0,
        handles.sp1,
        20,
        2,
        read_snap,
        write_snap,
        view_sp1_reg0_snaph,
        (void*)0x1,
        True
    );

}

/////////////////////////////////// Net Ports //////////////////////////////////

static void installNetPorts(void) {
// To write to this net, use ppmWriteNet(handles.irq, value);

    handles.irq = ppmOpenNetPort("irq");

// To write to this net, use ppmWriteNet(handles.resetrequest, value);

    handles.resetrequest = ppmOpenNetPort("resetrequest");

// To write to this net, use ppmWriteNet(handles.timeout_pulse, value);

    handles.timeout_pulse = ppmOpenNetPort("timeout_pulse");

}

PPM_NET_CB(reset_IRESET) {
    if(value != 0 ) {
        sp1_reg0_data.snaph.value = 0x0;
        sp1_reg0_data.snapl.value = 0x0;
        sp1_reg0_data.periodh.value = 0x0;
        sp1_reg0_data.periodl.value = 0x0;
        sp1_reg0_data.control.value = 0x0;
        sp1_reg0_data.status.value = 0x0;
    }
}

////////////////////////////////// Constructor /////////////////////////////////

PPM_CONSTRUCTOR_CB(periphConstructor) {
    installSlavePorts();
    installRegisters();
    installNetPorts();
    reset_IRESET(1,0);
}

///////////////////////////////////// Main /////////////////////////////////////

int main(int argc, char *argv[]) {

    ppmDocNodeP doc1_node = ppmDocAddSection(0, "Licensing");
    ppmDocAddText(doc1_node, "Open Source Apache 2.0");
    ppmDocNodeP doc_11_node = ppmDocAddSection(0, "Description");
    ppmDocAddText(doc_11_node, "Altera Avalon Interval Timer32 Core");
    ppmDocNodeP doc_21_node = ppmDocAddSection(0, "Limitations");
    ppmDocAddText(doc_21_node, "No Support for pin level transitions");

    diagnosticLevel = 0;
    bhmInstallDiagCB(setDiagLevel);
    constructor();

    return 0;
}

