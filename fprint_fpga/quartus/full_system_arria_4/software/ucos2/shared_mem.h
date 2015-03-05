/*
 * shared_mem.h
 *
 *  Created on: 2013-11-08
 *      Author: Jonah
 */

#ifndef SHARED_MEM_H_
#define SHARED_MEM_H_


typedef struct CriticalFunctionPointers{
	//Priority of the critical task
	unsigned int priority[8];
	//The pointer to the function code
	void *task[8];
	//Arguments to the function
	void *args[8];
	//To pass timing results back from the cores to the monitor
	alt_u64  core_time[8];
	//Monitor signals to cores to begin initializing in main
	int init_complete;
	//Cores signal that they are done
	int core_ready[8];
	//Cores signal to each other that they are ready
	int checkout[8];
	//Cores find out who their partner is
	int partner[8];
	//blocksize for fingerprinting
	int blocksize[8];

	int timelog[4][6];

}CriticalFunctionPointers;


#endif /* SHARED_MEM_H_ */
