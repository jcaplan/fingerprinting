
#include <stdbool.h>

#define FPRINT_VALID 1
#define FPRINT_EMPTY 0


#define COMPARATOR_BASE_ADDRESS 0x1000000
#define NUMCORES 2

#define CURRENT_STATE_REG 	0x8100000
#define MAX_COUNT_REG 		0x8100010
#define PAUSE_REG 			0x8100004
#define PAUSE_STROBE 		0x8100008
#define UNPAUSE_STROBE 		0x810000C


#define PROCESSOR0_0_SW_RESET 0x3000000
#define PROCESSOR1_0_SW_RESET 0x3000400

void enableFingerprinting(Uns32 coreID, Uns32 taskID);
void disableFingerprinting(Uns32 coreID, Uns32 taskID);
bool getFprintEnabled(Uns32 coreID);
void do_store(Uns32 coreID, vmiProcessorP processor, Uns32 address, Uns32 data);
void setMaxCount(Uns32 coreID, Uns32 writeData);
void fprintInit(Uns32 coreID, vmiProcessorP processor);
void fprintReset(Uns32 coreID);
void fprintPauseStrobe(int coreID);
void fprintUnpauseStrobe(int coreID);