#ifndef SHARED_MEM_TESTING_H_
#define SHARED_MEM_TESTING_H_

#define NUM_CORES 8

typedef struct CriticalFunctionPointers{
	void* task_pt;
	unsigned int init_complete;
	unsigned short fprint_blocksize[NUM_CORES];
	unsigned short fprint_enable[NUM_CORES];
	unsigned short task_id[NUM_CORES];
	unsigned short task_length[NUM_CORES];
	unsigned short task_complete[NUM_CORES];
	unsigned int oflow_count[NUM_CORES];
	unsigned int checkout[NUM_CORES];
	unsigned int checkin[2];
	unsigned int core_ready[NUM_CORES];
	alt_u64  core_total_time[NUM_CORES];
	alt_u64  core_oflow_time[NUM_CORES];
}CriticalFunctionPointers;


#endif
