/*
 * Processor.cpp
 *
 *  Created on: Feb 28, 2015
 *      Author: jonah
 */

#include <stdlib.h>
#include "Processor.h"
#include "galloc.h"
/**********************************************************
 * Constructors
 **********************************************************/
Processor::Processor() {
	parameters = (ParamP)galloc(1,sizeof(Param));
	parameters->faultTolerant = false;
	parameters->scratchpadSize = 16384;
	parameters->timePeriodMS = 10;
	parameters->hasFprint = true;
	parameters->hasICache = true;
	parameters->hasDCache = false;
	parameters->hasDMA = true;
	parameters->sharedMemorySize = 1024;
}

Processor::Processor(ParamP p){
	parameters = p;
}

/**********************************************************
 * Destructor
 **********************************************************/
Processor::~Processor() {
	// TODO Auto-generated destructor stub
}
