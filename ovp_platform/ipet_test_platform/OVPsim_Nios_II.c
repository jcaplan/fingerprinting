/*
 * Copyright (c) 2005-2015 Imperas Software Ltd., www.imperas.com
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied.
 *
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "icm/icmCpuManager.h"

// valid command line
const char *usage = "<application name> [GDB port]";
// Variables set by arguments
char *application;              // the application to load
Bool enableDebug = False;       // set True when debugging selected
Bool autoGDBConsole = False;    // set True when auto start of GDB console selected
Uns32 portNum = 0;              // set to a port number for a debug connection

icmProcessorP createPlatform(void)
{

////////////////////////////////////////////////////////////////////////////////

    // initialize OVPsim, enabling verbose mode to get statistics at end of execution
    // and
    // Imperas Intercepts to utilise specific builtin simulator functions
    unsigned int icmAttrs = ICM_VERBOSE | ICM_STOP_ON_CTRLC;

    if(autoGDBConsole) {
        icmAttrs |= ICM_GDB_CONSOLE;
    }
    icmInitPlatform(ICM_VERSION, icmAttrs, enableDebug ? "rsp" : 0, portNum, "platform");

#define SIM_ATTRS (ICM_ATTR_DEFAULT)

    const char *nios_iiModel =
        icmGetVlnvString(NULL, "altera.ovpworld.org", "processor", "nios_ii",
                               "1.0", "model");
    const char *nios_iiSemihost =
        icmGetVlnvString(NULL, "altera.ovpworld.org", "semihosting",
                               "nios_iiNewlib", "1.0", "model");

    icmAttrListP userAttrs = icmNewAttrList();
    icmAddStringAttr(userAttrs, "variant", "Nios_II_S");
    icmAddUns64Attr(userAttrs,  "HW_DIVIDE", 1);
    icmAddUns64Attr(userAttrs,  "HW_MULTIPLY", 1);

    // create a processor instance
    icmProcessorP processor = icmNewProcessor(
            "cpu0",           // CPU name
            "nios_ii",        // CPU type
            0,                // CPU cpuId
            0,                // CPU model flags
            32,               // address bits
            nios_iiModel,     // model file
            "modelAttrs",     // morpher attributes
            SIM_ATTRS,        // attributes
            userAttrs,        // user-defined attributes
            nios_iiSemihost,  // semi-hosting file
            "modelAttrs"      // semi-hosting attributes
        );

    // create the processor bus
    icmBusP bus = icmNewBus("bus", 32);

    // connect the processors onto the busses
    icmConnectProcessorBusses(processor, bus, bus);

    // create memory
    icmMemoryP local = icmNewMemory("local", ICM_PRIV_RWX, 0xffffffff);

    // connect the memory onto the busses
    icmConnectMemoryToBus(bus, "mp1", local, 0x00000000);

    return processor;
}

////////////////////////////////////////////////////////////////////////////////
//                                   M A I N                                  //
////////////////////////////////////////////////////////////////////////////////

int
main(int argc, char *argv[])
{

    // the constructor
    icmProcessorP processor = createPlatform();

    // load the application executable file into processor memory space
    if(!icmLoadProcessorMemory(processor, "/home/jonah/fingerprinting/code_gen/CodeGen/test/ipetTest/ipet.out", ICM_LOAD_DEFAULT, False, True)) {
        return -1;
    }
    // run till the end
    icmSimulatePlatform();

    // finish and clean up
    icmTerminate();

    return 0;
}

////////////////////////////////////////////////////////////////////////////////

