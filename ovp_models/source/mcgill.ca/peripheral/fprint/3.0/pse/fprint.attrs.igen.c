
////////////////////////////////////////////////////////////////////////////////
//
//                W R I T T E N   B Y   I M P E R A S   I G E N
//
//                             Version 20150205.0
//                          Tue Jun  9 12:42:55 2015
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
        .name            = "FPRINT_CSR_SLAVE",
        .type            = PPM_SLAVE_PORT,
        .addrHi          = 0x13fLL,
        .mustBeConnected = 1,
        .remappable      = 0,
        .description     = "Fingerprint control register slave port",
    },
    {
        .name            = "FPRINT_TLB_SLAVE",
        .type            = PPM_SLAVE_PORT,
        .addrHi          = 0x3ffLL,
        .mustBeConnected = 0,
        .remappable      = 0,
        .description     = "Get translations from TLB",
    },
    {
        .name            = "FPRINT_MASTER",
        .type            = PPM_MASTER_PORT,
        .addrBits        = 32,
        .mustBeConnected = 0,
        .description     = "Fingerprint master port",
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
        .name            = "FPRINT_WRITE",
        .type            = PPM_INPUT_PORT,
        .mustBeConnected = 0,
        .description     = "bus write signal"
    },
    {
        .name            = "FPRINT_WRITE_ADDRESS",
        .type            = PPM_INPUT_PORT,
        .mustBeConnected = 0,
        .description     = "bus address signal"
    },
    {
        .name            = "FPRINT_WRITE_DATA",
        .type            = PPM_INPUT_PORT,
        .mustBeConnected = 0,
        .description     = "bus address signal"
    },
    {
        .name            = "FPRINT_RESET",
        .type            = PPM_INPUT_PORT,
        .mustBeConnected = 0,
        .description     = "reset signal"
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
    {
        .name        = "coreID",
        .type        = ppm_PT_UNS32,
        .description = 0,
        .u.uns32Param.max          = 15,
    },
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
        .name    = "fprint",
        .version = "2.0"
    },

    .family    = "mcgill.ca",           

};
