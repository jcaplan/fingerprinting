/*
 * shared_mem.h
 *
 *  Created on: 2013-11-08
 *      Author: Jonah
 */

#ifndef SHARED_MEM_H_
#define SHARED_MEM_H_


typedef struct CriticalFunctionPointers{
	void* gpm;
	void* critical;
	void* preempt;
	unsigned short task_id0;
	unsigned short task_id1;
	unsigned short task_id2;
	unsigned short task_id3;
	unsigned short task_id4;
	unsigned short task_id5;
	unsigned short task_id6;
	unsigned short task_id7;
	unsigned int init_complete;
	unsigned int checkout0;
	unsigned int checkout1;
	unsigned int checkout2;
	unsigned int checkout3;
	unsigned int checkout4;
	unsigned int checkout5;
	unsigned int checkout6;
	unsigned int checkout7;
	unsigned int checkin0;
	unsigned int checkin1;
	unsigned int core0_ready;
	unsigned int core1_ready;
	unsigned int core2_ready;
	unsigned int core3_ready;
	unsigned int core4_ready;
	unsigned int core5_ready;
	unsigned int core6_ready;
	unsigned int core7_ready;
	alt_u64  core_time[8];

}CriticalFunctionPointers;


#endif /* SHARED_MEM_H_ */
