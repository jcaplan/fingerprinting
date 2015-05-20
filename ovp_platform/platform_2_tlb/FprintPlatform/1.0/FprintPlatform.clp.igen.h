
////////////////////////////////////////////////////////////////////////////////
//
//                W R I T T E N   B Y   I M P E R A S   I G E N
//
//                             Version 20140430.0
//                          Wed May 20 15:51:44 2015
//
////////////////////////////////////////////////////////////////////////////////

// This file constructs and runs the command line parser.
// This file should NOT need to be edited.

static Bool cmdParser(int argc, const char *argv[]) {
    icmCLPP parser = icmCLParser("FprintPlatform", ICM_AC_ALL);
    return icmCLParseArgs(parser, argc, argv);
}
