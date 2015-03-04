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

//
// Read callback - triggered on every memory read
//
static VMI_MEM_WATCH_FN(readCB) {

    // we are interested only in reads made by processors (not artifacts of
    // simulation or memory accesses by other plugins, for example) so take
    // action only if processor is non null
    if(processor) {

        vmiosObjectP object = userData;

        // vmiPrintf(
        //     "*** mtrace-plugin: cpu %s reads %u bytes (after "FMT_64u" instructions)\n",
        //     vmirtProcessorName(processor),
        //     bytes,
        //     vmirtGetICount(processor)
        // );

        object->totalReads++;
        object->totalReadBytes += bytes;

    }
}

//
// Write callback - triggered on every memory write
//
int count = 0;
static VMI_MEM_WATCH_FN(writeCB) {

    // we are interested only in writes made by processors (not artifacts of
    // simulation or memory accesses by other plugins, for example) so take
    // action only if processor is non null
    if(processor) {

        vmiosObjectP object = userData;

        // vmiPrintf(
        //     "%s writes data 0x%08x (after "FMT_64u" instructions) to address 0x"
        //     FMT_Ax"\n",
        //     vmirtProcessorName(processor),
        //     (unsigned int) *((unsigned *)value),
        //     vmirtGetICount(processor),
        //     address
        // );
       //vmiPrintf("%d\n",vmirtConnGetInput(processor, "ibus",32));
        object->totalWrites++;
        object->totalWriteBytes += bytes;

        // trigger watchdog signal
        //vmiPrintf("processor %s writes %x to %x\n", vmirtProcessorName(processor),*((unsigned *)value),(int)address);        
        
        vmirtWriteNetPort(processor, vmirtGetNetPortHandle(processor, "fprint_write_out_address"), address);
        vmirtWriteNetPort(processor, vmirtGetNetPortHandle(processor, "fprint_write_out_data"), *((unsigned *)value));
        vmirtWriteNetPort(processor, vmirtGetNetPortHandle(processor, "fprint_write_out"), 1);



    }
}

//
// Constructor - install read and write callbacks on processor data domain
//
static VMIOS_CONSTRUCTOR_FN(constructor) {

    memDomainP domain   = vmirtGetProcessorPhysicalDataDomain(processor);
    Uns8       bits     = vmirtGetDomainAddressBits(domain);
    Addr       highAddr = ((bits==64) ? 0ULL : (1ULL<<bits)) - 1;

    vmirtAddReadCallback(domain, 0, 0, highAddr, readCB, object);
    vmirtAddWriteCallback(domain, 0, 0, highAddr, writeCB, object);
}

//
// Destructor
//
static VMIOS_DESTRUCTOR_FN(destructor) {
/*
    vmiPrintf("*** mtrace-plugin: cpu %s:\n", vmirtProcessorName(processor));
    vmiPrintf("  number of reads:  %u\n", object->totalReads);
    vmiPrintf("  number of writes: %u\n", object->totalWrites);
    vmiPrintf("  bytes read:       %u\n", object->totalReadBytes);
    vmiPrintf("  bytes written:    %u\n", object->totalWriteBytes);*/
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
