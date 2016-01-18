
////////////////////////////////////////////////////////////////////////////////
//
//                W R I T T E N   B Y   I M P E R A S   I G E N
//
//                             Version 20150205.0
//                          Sun Jan 17 21:01:45 2016
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
        .name            = "sp1",
        .type            = PPM_SLAVE_PORT,
        .addrHi          = 0x17LL,
        .mustBeConnected = 0,
        .remappable      = 0,
        .description     = 0,
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
        .name            = "irq",
        .type            = PPM_OUTPUT_PORT,
        .mustBeConnected = 0,
        .description     = 0
    },
    {
        .name            = "resetrequest",
        .type            = PPM_OUTPUT_PORT,
        .mustBeConnected = 0,
        .description     = 0
    },
    {
        .name            = "timeout_pulse",
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

static ppmEnumParameter timeoutConfigValues[] = {
    { .name = "Simple", .value = 0},
    { .name = "Full", .value = 1},
    { .name = "Watchdog", .value = 2},
    { 0 }
};

static ppmParameter parameters[] = {
    {
        .name        = "timeoutPeriod",
        .type        = ppm_PT_UNS64,
        .description = 0,
    },
    {
        .name        = "timerFrequency",
        .type        = ppm_PT_UNS64,
        .description = 0,
    },
    {
        .name        = "timeoutConfig",
        .type        = ppm_PT_ENUM,
        .description = 0,
        .u.enumParam.legalValues = timeoutConfigValues,
    },
    {
        .name        = "writeablePeriod",
        .type        = ppm_PT_BOOL,
        .description = 0,
        .u.boolParam.defaultValue = 0,
    },
    {
        .name        = "readableSnapshot",
        .type        = ppm_PT_BOOL,
        .description = 0,
        .u.boolParam.defaultValue = 0,
    },
    {
        .name        = "startStopControlBits",
        .type        = ppm_PT_BOOL,
        .description = 0,
        .u.boolParam.defaultValue = 0,
    },
    {
        .name        = "timeoutPulse",
        .type        = ppm_PT_BOOL,
        .description = 0,
        .u.boolParam.defaultValue = 0,
    },
    {
        .name        = "systemResetOnTimeout",
        .type        = ppm_PT_BOOL,
        .description = 0,
        .u.boolParam.defaultValue = 0,
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
        .name    = "IntervalTimer32Core",
        .version = "1.0"
    },

    .family    = "mcgill.ca",           

};
