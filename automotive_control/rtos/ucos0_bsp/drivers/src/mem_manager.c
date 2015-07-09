#include "mem_manager.h"

#ifndef NULL
#define NULL ((void *) 0)
#endif

bool requires_manager(int task){
	MemoryManagerStruct *tableEntry = getTaskEntry(task);

	if(tableEntry != NULL)
		return true;
	else
		return false;
}




void updateMemoryManagerTable(int index, CriticalFunctionData *data){

	MemoryManagerStruct *entry = &memoryManagerTable[index];

	entry->dataVirtualAddress = data->tlbDataAddressVirt;
	entry->dataPhysicalAddress = data->tlbDataAddressPhys;
	entry->stackVirtualAddress = data->tlbStackAddressVirt;
	entry->stackPhysicalAddress = data->tlbStackAddressPhys;

}

MemoryManagerStruct *getTaskEntry(int task){
	int i;
	for(i = 0; i < OS_MAX_TASKS; i++){
		if(memoryManagerTable[i].taskPriority == task){
			return &memoryManagerTable[i];
		}
	}

	return NULL;
}


void managerDisableCurrentTask(INT8U OSPrioCur){
	MemoryManagerStruct *tableEntry = getTaskEntry(OSPrioCur);
	if(tableEntry == NULL){
		return;
	}
	tableEntry->disablePending = true;
}

void managerEnableNextTask(INT8U OSPrioHighRdy){

	MemoryManagerStruct *entry = &memoryManagerTable[derivate_airbagModel_memoryTableIndex];

	switch(OSPrioHighRdy){
	case Derivative_AirbagModel_PRIORITY:
		if(entry->stackVirtualAddress != 0){
			set_cputable_entry(entry->tlbStackLine, entry->stackVirtualAddress);
			set_spmtable_entry(entry->tlbStackLine, entry->stackPhysicalAddress);
			enableTlbLine(entry->tlbStackLine);
		}

		if(entry->dataVirtualAddress != 0){
			set_cputable_entry(entry->tlbDataLine, entry->dataVirtualAddress);
			set_spmtable_entry(entry->tlbDataLine, entry->dataPhysicalAddress);
			enableTlbLine(entry->tlbDataLine);
		}

		activateTlb();
		break;
	default:
		break;
	}


}


void  managerCheckPendingDisabled(int OSPrioCur){
	int i;
	for(i = 0; i < OS_MAX_TASKS; i++){
		MemoryManagerStruct *entry = &memoryManagerTable[i];
		if(entry->disablePending && entry->taskPriority != OSPrioCur){
			disableTlbLine(entry->tlbDataLine);
			disableTlbLine(entry->tlbStackLine);
			entry->disablePending = false;
		}
	}
}
