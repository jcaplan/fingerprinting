
////////////////////////////////////////////////////////////////////////////////
//
//                W R I T T E N   B Y   I M P E R A S   I G E N
//
//                             Version 20140430.0
//                          Thu Sep  4 17:29:59 2014
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////// Description /////////////////////////////////

// CPU IRQ peripheral model


#include "cpu_irq.igen.h"
/////////////////////////////// Port Declarations //////////////////////////////

CPU_IRQ_SLAVE_ab32_dataT CPU_IRQ_SLAVE_ab32_data;

handlesT handles;

/////////////////////////////// Diagnostic level ///////////////////////////////

// Test this variable to determine what diagnostics to output.
// eg. if (diagnosticLevel > 0) bhmMessage("I", "cpu_irq", "Example");

Uns32 diagnosticLevel;

/////////////////////////// Diagnostic level callback //////////////////////////

static void setDiagLevel(Uns32 new) {
    diagnosticLevel = new;
}

///////////////////////////// MMR Generic callbacks ////////////////////////////

static PPM_VIEW_CB(view32) {  *(Uns32*)data = *(Uns32*)user; }

//////////////////////////////// Bus Slave Ports ///////////////////////////////

static void installSlavePorts(void) {
    handles.CPU_IRQ_SLAVE = ppmCreateSlaveBusPort("CPU_IRQ_SLAVE", 9216);
    if (!handles.CPU_IRQ_SLAVE) {
        bhmMessage("E", "PPM_SPNC", "Could not connect port 'CPU_IRQ_SLAVE'");
    }

}

//////////////////////////// Memory mapped registers ///////////////////////////

static void installRegisters(void) {

    ppmCreateRegister("ab32_cpum",
        0,
        handles.CPU_IRQ_SLAVE,
        2048,
        4,
        regRd32,
        regWr32,
        view32,
        &(CPU_IRQ_SLAVE_ab32_data.cpum.value),
        True
    );
    ppmCreateRegister("ab32_cpu0",
        0,
        handles.CPU_IRQ_SLAVE,
        0,
        4,
        regRd32,
        regWr32,
        view32,
        &(CPU_IRQ_SLAVE_ab32_data.cpu0.value),
        True
    );
    ppmCreateRegister("ab32_cpu1",
        0,
        handles.CPU_IRQ_SLAVE,
        1024,
        4,
        regRd32,
        regWr32,
        view32,
        &(CPU_IRQ_SLAVE_ab32_data.cpu1.value),
        True
    );
    ppmCreateRegister("ab32_cpu2",
        0,
        handles.CPU_IRQ_SLAVE,
        3072,
        4,
        regRd32,
        regWr32,
        view32,
        &(CPU_IRQ_SLAVE_ab32_data.cpu2.value),
        True
    );
    ppmCreateRegister("ab32_cpu3",
        0,
        handles.CPU_IRQ_SLAVE,
        4096,
        4,
        regRd32,
        regWr32,
        view32,
        &(CPU_IRQ_SLAVE_ab32_data.cpu3.value),
        True
    );
    ppmCreateRegister("ab32_cpu4",
        0,
        handles.CPU_IRQ_SLAVE,
        5120,
        4,
        regRd32,
        regWr32,
        view32,
        &(CPU_IRQ_SLAVE_ab32_data.cpu4.value),
        True
    );
    ppmCreateRegister("ab32_cpu5",
        0,
        handles.CPU_IRQ_SLAVE,
        6144,
        4,
        regRd32,
        regWr32,
        view32,
        &(CPU_IRQ_SLAVE_ab32_data.cpu5.value),
        True
    );
    ppmCreateRegister("ab32_cpu6",
        0,
        handles.CPU_IRQ_SLAVE,
        7168,
        4,
        regRd32,
        regWr32,
        view32,
        &(CPU_IRQ_SLAVE_ab32_data.cpu6.value),
        True
    );
    ppmCreateRegister("ab32_cpu7",
        0,
        handles.CPU_IRQ_SLAVE,
        8192,
        4,
        regRd32,
        regWr32,
        view32,
        &(CPU_IRQ_SLAVE_ab32_data.cpu7.value),
        True
    );

}

/////////////////////////////////// Net Ports //////////////////////////////////

static void installNetPorts(void) {
// To write to this net, use ppmWriteNet(handles.cpum_irq, value);

    handles.cpum_irq = ppmOpenNetPort("cpum_irq");

// To write to this net, use ppmWriteNet(handles.cpu0_irq, value);

    handles.cpu0_irq = ppmOpenNetPort("cpu0_irq");

// To write to this net, use ppmWriteNet(handles.cpu1_irq, value);

    handles.cpu1_irq = ppmOpenNetPort("cpu1_irq");

// To write to this net, use ppmWriteNet(handles.cpu2_irq, value);

    handles.cpu2_irq = ppmOpenNetPort("cpu2_irq");

// To write to this net, use ppmWriteNet(handles.cpu3_irq, value);

    handles.cpu3_irq = ppmOpenNetPort("cpu3_irq");

// To write to this net, use ppmWriteNet(handles.cpu4_irq, value);

    handles.cpu4_irq = ppmOpenNetPort("cpu4_irq");

// To write to this net, use ppmWriteNet(handles.cpu5_irq, value);

    handles.cpu5_irq = ppmOpenNetPort("cpu5_irq");

// To write to this net, use ppmWriteNet(handles.cpu6_irq, value);

    handles.cpu6_irq = ppmOpenNetPort("cpu6_irq");

// To write to this net, use ppmWriteNet(handles.cpu7_irq, value);

    handles.cpu7_irq = ppmOpenNetPort("cpu7_irq");

}

////////////////////////////////// Constructor /////////////////////////////////

PPM_CONSTRUCTOR_CB(periphConstructor) {
    installSlavePorts();
    installRegisters();
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

