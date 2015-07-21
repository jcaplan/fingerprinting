
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
	Uns32 count[8];
	Uns32 count_old;
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
	Uns32  address = COMPARATOR_BASE_ADDRESS + (((coreID << 4) + 0) << 2);
	vmiPrintf("INTERCEPT: sending to comparator: %x, %x\n",address,taskID);
	vmirtWrite4ByteDomain(fp->domain,address,fp->endian,taskID,MEM_AA_FALSE);
	vmiPrintf("INTERCEPT: fingerprinting enabled task %d on core %d\n",fp->currentTask,coreID);
}

void disableFingerprinting(Uns32 coreID, Uns32 taskID){
	fprintStruct *fp = &fprint[coreID];
	fp->currentTask = taskID & 0xf;
	fp->fprintEnabled = false;

	Uns32 address;
	address = COMPARATOR_BASE_ADDRESS + (((coreID << 4) + 1) << 2);
	vmiPrintf("INTERCEPT: sending to comparator: %x, %x\n",address,fp->fprint[fp->pause_index]);
	vmirtWrite4ByteDomain(fp->domain,address,fp->endian,fp->fprint[fp->pause_index],MEM_AA_FALSE);

	address = COMPARATOR_BASE_ADDRESS + (((coreID << 4) + 0) << 2);
	vmiPrintf("INTERCEPT: sending to comparator: %x, %x\n",address,taskID);
	vmirtWrite4ByteDomain(fp->domain,address,fp->endian,taskID,MEM_AA_FALSE);
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
	if(fp->fprint_pause_hold[0] == FPRINT_VALID){
		Uns32 fprint_address = COMPARATOR_BASE_ADDRESS + (((coreID << 4) + 1) << 2);
		vmirtWrite4ByteDomain(fp->domain, fprint_address, fp->endian, fp->fprint_pause_hold[1], MEM_AA_FALSE);
		fp->fprint_pause_hold[0] = FPRINT_EMPTY;
		vmiPrintf("core %d completed fingerprint: %x\n",coreID, fp->fprint_pause_hold[1]);
	}

	fp->fprint_old = fp->fprint[fp->pause_index];
	fp->fprint[fp->pause_index] = crcFast(address,data,fp->fprint[fp->pause_index]);
	vmiPrintf("INTERCEPT: old fingerprint: %x,  intermediate fingerprint: %x, pause_index: %d\n",
		fp->fprint_old,fp->fprint[fp->pause_index],fp->pause_index);
	fp->count_old = fp->count[fp->pause_index];
	if(fp->mode == 1){
		fp->count[fp->pause_index] += 2;
	} else {
		fp->count[fp->pause_index] += 1;
	}

	if(fp->count[fp->pause_index] >= fp->countMax){
		fp->fprint_pause_hold[0] = FPRINT_VALID;
		fp->fprint_pause_hold[1] = fp->fprint[fp->pause_index];
		fp->fprint[fp->pause_index] = 0;
		fp->count[fp->pause_index] = 0;
	}
}

void setMaxCount(Uns32 coreID, Uns32 writeData) {
	fprint[coreID].countMax = writeData;
}


void pause(Uns32 coreID) {
	fprintStruct *fp = &fprint[coreID];
	fp->fprint_pause_hold[0] = FPRINT_EMPTY;
	fp->fprint[fp->pause_index] = fp->fprint_old;
	fp->count[fp->pause_index]  = fp->count_old;
	fp->currentState[fp->pause_index] = fp->currentTask;
	Uns32 address = COMPARATOR_BASE_ADDRESS + (((coreID << 4) + 2) << 2);
	vmirtWrite4ByteDomain(fp->domain, address, fp->endian, fp->currentState[fp->pause_index], MEM_AA_FALSE);
	fp->pause_index++;
	fp->fprint[fp->pause_index] = 0;
	fp->count[fp->pause_index]  = 0;
	fp->fprintEnabled  = false;
	fp->currentTask = 0;
}

void unpause(Uns32 coreID) {
	fprintStruct *fp = &fprint[coreID];
	if(fp->pause_index > 0){
  		fp->pause_index--;
		fp->currentTask = fp->currentState[fp->pause_index];
    	Uns32 address = COMPARATOR_BASE_ADDRESS + (((coreID << 4) + 3) << 2);
	vmirtWrite4ByteDomain(fp->domain, address, fp->endian, fp->currentState[fp->pause_index], MEM_AA_FALSE);
	}
}


void fprintReset(Uns32 coreID){

	fprintStruct *fp = &fprint[coreID];

	vmiPrintf("Resetting Fprint %d!\n",coreID);
	int i;
	for(i = 0; i < 8; i++){
		fp->fprint[i] = 0;
		fp->count[i] = 0;
		fp->currentState[i] = 0;
	}
	fp->fprint_old = 0;
	fp->count_old = 0;
	fp->pauseReg = 0;
	fp->pause_index = 0;
	fp->mode = 0;

	
	fp->currentTask = 0;
	fp->countMax = 0;
	fp->fprintEnabled = false;
	fp->fprint_pause_hold[0] = FPRINT_EMPTY;
	fp->fprint_pause_hold[1] = 1;

}

