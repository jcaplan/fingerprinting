
////////////////////////////////////////////////////////////////////////////////
//
//                W R I T T E N   B Y   I M P E R A S   I G E N
//
//                             Version 20140430.0
//                          Tue Jul 29 17:25:54 2014
//
////////////////////////////////////////////////////////////////////////////////

#include "crc.h"
#include "fprint.h"

//////////////////////////////// Callback stubs ////////////////////////////////

typedef struct {
	Uns32 fprint[8];
	Uns32 fprint_old;
	Uns32 pauseReg;
	Uns32 currentState[8];
	Uns32 pause_index;
	Uns32 coreID;
	Uns32 mode;
	bool fprintEnabled;
	Uns32 currentTask;
	Uns32 fprint_pause_hold[2];
	memEndian endian;
	memDomainP domain;
	Uns32 countMax;
} fprintStruct;

fprintStruct fprint[NUMCORES];

	
void enableFingerprinting(Uns32 coreID, Uns32 taskID){

	fprintStruct *fp = &fprint[coreID];
	fp->currentTask = taskID & 0xf;
	fp->fprintEnabled = true;
	Uns32 address = COMPARATOR_BASE_ADDRESS;
	Uns32 data = (coreID << 5) | 0x10 | taskID;
	vmiPrintf("INTERCEPT: sending to comparator: %x, %x\n",address,data);
	vmirtWrite4ByteDomain(fp->domain,address,fp->endian,data,MEM_AA_FALSE);
	vmiPrintf("INTERCEPT: fingerprinting enabled task %d on core %d\n",fp->currentTask,coreID);
}

void disableFingerprinting(Uns32 coreID, Uns32 taskID){
	fprintStruct *fp = &fprint[coreID];
	fp->currentTask = taskID & 0xf;
	fp->fprintEnabled = false;

	Uns32 address;
	Uns32 data;
	address = COMPARATOR_BASE_ADDRESS + 0x4;
	data = ((fp->fprint[fp->pause_index] & 0xFFFF) << 16) | (coreID << 4) | taskID;
	vmiPrintf("INTERCEPT: sending to comparator: %x, %x\n",address,data);
	vmirtWrite4ByteDomain(fp->domain,address,fp->endian,data,MEM_AA_FALSE);
	data = ((fp->fprint[fp->pause_index] & 0xFFFF0000)) | (0x100) | (coreID << 4) | taskID;
	vmiPrintf("INTERCEPT: sending to comparator: %x, %x\n",address,data);
	vmirtWrite4ByteDomain(fp->domain,address,fp->endian,data,MEM_AA_FALSE);


	address = COMPARATOR_BASE_ADDRESS;
	data = (coreID << 5) | taskID;
	vmiPrintf("INTERCEPT: sending to comparator: %x, %x\n",address,data);
	vmirtWrite4ByteDomain(fp->domain,address,fp->endian,data,MEM_AA_FALSE);
	vmiPrintf("INTERCEPT: fingerprinting disabled task %d on core %d\n",taskID,coreID);
}


bool getFprintEnabled(Uns32 coreID){
	return fprint[coreID].fprintEnabled;
}


void fprintInit(Uns32 coreID, vmiProcessorP processor){
	
		fprintStruct *fp = &fprint[coreID];
		fp->fprintEnabled = false;
		fp->endian = vmirtGetProcessorDataEndian(processor);
		fp->domain = vmirtGetProcessorPhysicalDataDomain(processor);
		fp->pause_index = 0;

}


void do_store(Uns32 coreID, vmiProcessorP processor, Uns32 address, Uns32 data){

	fprintStruct *fp = &fprint[coreID];

	fp->fprint_old = fp->fprint[fp->pause_index];
	fp->fprint[fp->pause_index] = crcFast(address,data,fp->fprint[fp->pause_index]);
	vmiPrintf("INTERCEPT: old fingerprint: %x,  intermediate fingerprint: %x, pause_index: %d\n",
		fp->fprint_old,fp->fprint[fp->pause_index],fp->pause_index);
	
}

void setMaxCount(Uns32 coreID, Uns32 writeData) {
	fprint[coreID].countMax = writeData;
}


void pause(Uns32 coreID) {
	fprintStruct *fp = &fprint[coreID];
	fp->fprint_pause_hold[0] = FPRINT_EMPTY;
	fp->fprint[fp->pause_index] = fp->fprint_old;
	fp->currentState[fp->pause_index] = fp->currentTask;
	// Uns32 address = COMPARATOR_BASE_ADDRESS + (((coreID << 4) + 2) << 2);
	// vmirtWrite4ByteDomain(fp->domain, address, fp->endian, fp->currentState[fp->pause_index], MEM_AA_FALSE);
	fp->pause_index++;
	fp->fprint[fp->pause_index] = 0;
	fp->fprintEnabled  = false;
	fp->currentTask = 0;
}

void unpause(Uns32 coreID) {
	fprintStruct *fp = &fprint[coreID];
	if(fp->pause_index > 0){
  		fp->pause_index--;
		fp->currentTask = fp->currentState[fp->pause_index];
    	// Uns32 address = COMPARATOR_BASE_ADDRESS + (((coreID << 4) + 3) << 2);
	// vmirtWrite4ByteDomain(fp->domain, address, fp->endian, fp->currentState[fp->pause_index], MEM_AA_FALSE);
	}
}


void fprintReset(Uns32 coreID){

	fprintStruct *fp = &fprint[coreID];

	vmiPrintf("Resetting Fprint %d!\n",coreID);
	int i;
	for(i = 0; i < 8; i++){
		fp->fprint[i] = 0;
		fp->currentState[i] = 0;
	}
	fp->fprint_old = 0;
	fp->pauseReg = 0;
	fp->pause_index = 0;
	fp->mode = 0;

	fp->currentTask = 0;
	fp->countMax = 0;
	fp->fprintEnabled = false;
	fp->fprint_pause_hold[0] = FPRINT_EMPTY;
	fp->fprint_pause_hold[1] = 1;

}

