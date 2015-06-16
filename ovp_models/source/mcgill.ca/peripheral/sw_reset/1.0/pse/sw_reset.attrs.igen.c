
////////////////////////////////////////////////////////////////////////////////
//
//                W R I T T E N   B Y   I M P E R A S   I G E N
//
//                             Version 20150205.0
//                          Mon Jun  8 15:20:35 2015
//
////////////////////////////////////////////////////////////////////////////////


#ifdef _PSE_
#    include "peripheral/impTypes.h"
#    include "peripheral/bhm.h"
#    include "peripheral/ppm.h"
#else
#    include "hostapi/impTypes.h"
#endif


static ppmBusPort busPorts[] = {
    {
        .name            = "SW_RESET_SLAVE",
        .type            = PPM_SLAVE_PORT,
        .addrHi          = 0x23ffLL,
        .mustBeConnected = 1,
        .remappable      = 0,
        .description     = "SW Reset slave port",
    },
    { 0 }
};

static PPM_BUS_PORT_FN(nextBusPort) {
    if(!busPort) {
        busPort = busPorts;
    } else {
        busPort++;
    }
    return busPort->name ? busPort : 0;
}

static ppmNetPort netPorts[] = {
    {
        .name            = "cpum_reset",
        .type            = PPM_OUTPUT_PORT,
        .mustBeConnected = 0,
        .description     = 0
    },
    {
        .name            = "cpu0_reset",
        .type            = PPM_OUTPUT_PORT,
        .mustBeConnected = 0,
        .description     = 0
    },
    {
        .name            = "cpu1_reset",
        .type            = PPM_OUTPUT_PORT,
        .mustBeConnected = 0,
        .description     = 0
    },
    {
        .name            = "cpu2_reset",
        .type            = PPM_OUTPUT_PORT,
        .mustBeConnected = 0,
        .description     = 0
    },
    {
        .name            = "cpu3_reset",
        .type            = PPM_OUTPUT_PORT,
        .mustBeConnected = 0,
        .description     = 0
    },
    {
        .name            = "cpu4_reset",
        .type            = PPM_OUTPUT_PORT,
        .mustBeConnected = 0,
        .description     = 0
    },
    {
        .name            = "cpu5_reset",
        .type            = PPM_OUTPUT_PORT,
        .mustBeConnected = 0,
        .description     = 0
    },
    {
        .name            = "cpu6_reset",
        .type            = PPM_OUTPUT_PORT,
        .mustBeConnected = 0,
        .description     = 0
    },
    {
        .name            = "cpu7_reset",
        .type            = PPM_OUTPUT_PORT,
        .mustBeConnected = 0,
        .description     = 0
    },
    { 0 }
};

static PPM_NET_PORT_FN(nextNetPort) {
    if(!netPort) {
         netPort = netPorts;
    } else {
        netPort++;
    }
    return netPort->name ? netPort : 0;
}

static ppmParameter parameters[] = {
    { 0 }
};

static PPM_PARAMETER_FN(nextParameter) {
    if(!parameter) {
        parameter = parameters;
    } else {
        parameter++;
    }
    return parameter->name ? parameter : 0;
}

ppmModelAttr modelAttrs = {

    .versionString = PPM_VERSION_STRING,
    .type          = PPM_MT_PERIPHERAL,

    .busPortsCB    = nextBusPort,  
    .netPortsCB    = nextNetPort,  
    .paramSpecCB   = nextParameter,

    .vlnv          = {
        .vendor  = "mcgill.ca",
        .library = "peripheral",
        .name    = "sw_reset",
        .version = "1.0"
    },

    .family    = "mcgill.ca",           

};
