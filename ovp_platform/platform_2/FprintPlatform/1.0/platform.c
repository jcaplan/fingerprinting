
////////////////////////////////////////////////////////////////////////////////
//
//                W R I T T E N   B Y   I M P E R A S   I G E N
//
//                             Version 20140430.0
//                          Tue Aug 19 12:50:21 2014
//
////////////////////////////////////////////////////////////////////////////////


#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "icm/icmCpuManager.h"


// Be careful to avoid overwriting any edits if igen is run a second time.
// Only use -overwrite if you wish to overwrite this file.

#include "FprintPlatform.options.igen.h"
#include "FprintPlatform.handles.igen.h"
#include "FprintPlatform.constructor.igen.h"

static Bool postCmdParser(void) {
    // postCmdParser code goes here
    return True;
}
static int loadFile(icmProcessorP processor, char *fileName) {

    int i;
    
    icmPrintf("\nLoading file %s\n", fileName);
    
     char *file_contents;
     char *dataCheck;
     long input_file_size;
    FILE *input_file = fopen(fileName, "rb");
    
    // for(i = 0; < 1876; i++){
    //     fgetc    
    // }
    


     fseek(input_file, 0, SEEK_END);
     input_file_size = ftell(input_file);
     rewind(input_file);
     file_contents = malloc(input_file_size * (sizeof(char)));
     dataCheck = malloc(input_file_size * (sizeof(char)));    
     fread(file_contents, sizeof(char), input_file_size, input_file);

    icmWriteProcessorMemory(processor,      // processor
                            0x100000,        // memory address
                            file_contents,          // data buffer of data to write
                            input_file_size);             // number of bytes to write

     icmReadProcessorMemory(processor, 0x100000, dataCheck, input_file_size);

    for(i = 0; i < input_file_size; i++){
        if (file_contents[i] != dataCheck[i]){
            icmPrintf("Error! Data %d doesn't match\n", i);
            return False;
        }
    }
    
    icmPrintf("Load Complete\n\n");

    if (fclose(input_file)!=0) {
        icmPrintf ("Failed to close Memory Initialization File\n");
        return False;
    }

    free(file_contents);
    free(dataCheck);

    return True;
}

static Bool postPlatformConstruct(void) {
    //Load the file 
    
    return loadFile(handles.cpum_c, "archive.zip")
;
}

static Bool postSimulate(void) {
    // postSimulate code goes here
    return True;
}


#include "FprintPlatform.clp.igen.h"
////////////////////////////////////////////////////////////////////////////////
//                                   M A I N
////////////////////////////////////////////////////////////////////////////////

int main(int argc, const char *argv[]) {
    if(!cmdParser(argc, argv)) {
        return 1;
    }
    if(!postCmdParser())  {
        return 1;
    }
    platformConstructor();

    if(!postPlatformConstruct()) {
        return 1;
    }
    icmSimulatePlatform();
    if(!postSimulate()) {
        return 1;
    }
    icmTerminate();
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
