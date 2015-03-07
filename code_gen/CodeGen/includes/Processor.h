/*
 * Processor.h
 *
 *  Created on: Feb 28, 2015
 *      Author: jonah
 */

#ifndef PROCESSOR_H_
#define PROCESSOR_H_
#include "Port.h"
#include "Wire.h"
class Processor {
public:
	typedef struct Param{
		bool faultTolerant;
		int scratchpadSize; //Bytes
		int timePeriodMS; //Milliseconds
		bool hasFprint;
		bool hasICache;
		bool hasDCache;
		bool hasDMA;
		int sharedMemorySize; //Bytes
		} Param, *ParamP;

	Port ICache;
	Port DCache;
	Wire interrupts[32];

	ParamP parameters;

	Processor();
	Processor(ParamP p);

	~Processor();
};

#endif /* PROCESSOR_H_ */
