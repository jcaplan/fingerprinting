/*
 * shared_mem.h
 *
 *  Created on: 2013-11-08
 *      Author: Jonah
 */

#ifndef SHARED_MEM_H_
#define SHARED_MEM_H_

#define NUMCORES 2

//The table is determined statically by the generation tool
//It needs to be initialized at runtime but its size is known in advance.
//Declared in monitor main

#define DERIVATIVE_TASK_FID 1
#define DERIVATIVE_FUNC_TABLE_INDEX 0
#define AIRBAGMODEL_FUNC_TABLE_INDEX 1


typedef struct SharedMemorySymbolTable {
	struct FunctionTable *ftable;
	struct CriticalFunctionData *critFuncData;
	void *gp_address;
	//Monitor signals to cores to begin initializing in main
	int init_complete;
	//Cores signal that they are done
	int core_ready[NUMCORES];
} SharedMemorySymbolTable;


typedef struct FunctionTable {
	void *address;
	void *args;
	int blocksize;
	void *stackAddress[NUMCORES];
} FunctionTable;


//The format for the runtime message passed from monitor to cores
typedef struct CriticalFunctionData{
	//Priority of the critical task
	unsigned int priority;

	//The identity of the function is retrieved from the functionTable
	unsigned int tableIndex;

	//The TLB must be set appropriately for the function to run correctly
	//Each core locally manages its own TLB, only the start address is required
	unsigned int tlbTranslationPhys;
	unsigned int tlbTranslationVirt;
	int partnerCore;
	int checkout;


}CriticalFunctionData;


#endif /* SHARED_MEM_H_ */
