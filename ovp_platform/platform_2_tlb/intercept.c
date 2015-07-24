/*
 *
 * Copyright (c) 2005-2014 Imperas Software Ltd., www.imperas.com
 *
 * The contents of this file are provided under the Software License
 * Agreement that you accepted before downloading this file.
 *
 * This source forms part of the Software and can be used for educational,
 * training, and demonstration purposes but cannot be used for derivative
 * works except in cases where the derivative works require OVP technology
 * to run.
 *
 * For open source models released under licenses that you can use for
 * derivative works, please visit www.OVPworld.org or www.imperas.com
 * for the location of the open source models.
 *
 */


// VMI area includes
#include "vmi/vmiMessage.h"
#include "vmi/vmiOSAttrs.h"
#include "vmi/vmiRt.h"
#include "vmi/vmiPSE.h"
#include "vmi/vmiVersion.h"
#include "vmi/vmiTypes.h"
#include <string.h>
#include "crc.h"
#include "fprint.h"
#include <stdbool.h>

//This one is for the trace 2.mtrace

//
// Structure type containing local data for this plugin
//
typedef struct vmiosObjectS {
    Uns32 totalReads;
    Uns32 totalReadBytes;
    Uns32 totalWrites;
    Uns32 totalWriteBytes;
} vmiosObject;


int oldAddress = 0;
unsigned *old_value = 0;
static VMI_MEM_WATCH_FN(writeCB) {

    if(processor) {

        Uns32 coreID = vmirtCPUId(processor);    
        
        //current state register
        Uns32 writeData = *((unsigned *)value);

        switch(address){
        case CURRENT_STATE_REG:
            vmiPrintf("INTERCEPT: cpuid %d\n",coreID);
            if(writeData & 0x10){
                enableFingerprinting(coreID,writeData);
            } else {
               disableFingerprinting(coreID,writeData);
            }
            break;
        case MAX_COUNT_REG:
            vmiPrintf("INTERCEPT: cpuid %d\n",coreID);
            setMaxCount(coreID,writeData);
            break;
        case PROCESSOR0_0_SW_RESET:
            fprintReset(0);
            break;
        case PROCESSOR1_0_SW_RESET:
            fprintReset(1);
            break;
        default:
            break;
        }
        if(getFprintEnabled(coreID)){
            vmiPrintf("INTERCEPT: fingerprinting address: %x, data %x\n",(Uns32 )address,*(Uns32 *)value);

            do_store(coreID,processor,(Uns32)address, *(Uns32 *)value);

        }
    }
}

static VMI_MEM_WATCH_FN(readCB) {

    if(processor) {

        Uns32 coreID = vmirtCPUId(processor);    
        

        switch(address){
        case PAUSE_STROBE:
            fprintPauseStrobe(coreID);
            break;
        case UNPAUSE_STROBE:
            fprintUnpauseStrobe(coreID);
            break;
        case PAUSE_REG:

            break;
        default:
            break;
        }
    }
}


//
// Constructor - install read and write callbacks on processor data domain
//
static VMIOS_CONSTRUCTOR_FN(constructor) {

    const char *type = vmirtProcessorType(processor);
    Uns32 coreID = vmirtCPUId(processor);
    if(!strcmp(type,"nios_ii") && coreID < NUMCORES){
        memDomainP domain   = vmirtGetProcessorPhysicalDataDomain(processor);

        vmirtAddWriteCallback(domain, 0, 0, 0xffffffff, writeCB, object);
        vmirtAddReadCallback (domain, 0, 0, 0x08100400, readCB, object);

        vmiPrintf("INTERCEPT: initializing %s, %d\n",vmirtProcessorName(processor),coreID);
        crcInit();
        fprintInit(coreID, processor);

    }
}

//
// Destructor
//
static VMIOS_DESTRUCTOR_FN(destructor) {
}


////////////////////////////////////////////////////////////////////////////////
// INTERCEPT ATTRIBUTES
////////////////////////////////////////////////////////////////////////////////

vmiosAttr modelAttrs = {

    ////////////////////////////////////////////////////////////////////////
    // VERSION
    ////////////////////////////////////////////////////////////////////////

    .versionString = VMI_VERSION,            // version string
    .modelType     = VMI_INTERCEPT_LIBRARY,  // type
    .packageName   = "mtrace-plugin",        // description
    .objectSize    = sizeof(vmiosObject),    // size in bytes of OSS object

    ////////////////////////////////////////////////////////////////////////
    // CONSTRUCTOR/DESTRUCTOR ROUTINES
    ////////////////////////////////////////////////////////////////////////

    .constructorCB = constructor,            // object constructor
    .destructorCB  = destructor,             // object destructor

    ////////////////////////////////////////////////////////////////////////
    // ADDRESS INTERCEPT DEFINITIONS
    ////////////////////////////////////////////////////////////////////////

    .intercepts    = {{0}}

};
