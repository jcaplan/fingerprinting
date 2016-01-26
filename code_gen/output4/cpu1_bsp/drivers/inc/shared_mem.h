/*
 * shared_mem.h
 *
 *  Created on: 2013-11-08
 *      Author: Jonah
 */

#ifndef SHARED_MEM_H_
#define SHARED_MEM_H_

#define NUMCORES 2


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
	void *stackAddress[2];
} FunctionTable;


//The format for the runtime message passed from monitor to cores
typedef struct CriticalFunctionData{
	//Priority of the critical task
	unsigned int fprintID;

	//The identity of the function is retrieved from the functionTable
	unsigned int tableIndex;
	unsigned int modeChange;
	//The TLB must be set appropriately for the function to run correctly
	//Each core locally manages its own TLB, only the start address is required
	void *tlbDataAddressPhys;
	void *tlbDataAddressVirt;
	void *tlbStackAddressPhys;
	void *tlbStackAddressVirt;
	int partnerCore;
	int checkout;


}CriticalFunctionData;


#endif /* SHARED_MEM_H_ */
