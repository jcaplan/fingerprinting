
////////////////////////////////////////////////////////////////////////////////
//
//                W R I T T E N   B Y   I M P E R A S   I G E N
//
//                             Version 20150205.0
//                          Fri Jul 17 06:24:15 2015
//
////////////////////////////////////////////////////////////////////////////////

// This file constructs and runs the command line parser.
// This file should NOT need to be edited.

static Bool cmdParser(int argc, const char *argv[]) {
    icmCLPP parser = icmCLParser("FprintPlatform", ICM_AC_ALL);
    return icmCLParseArgs(parser, argc, argv);
}
