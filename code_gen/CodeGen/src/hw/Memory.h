/*
 * Memory.h
 *
 *  Created on: Feb 28, 2015
 *      Author: jonah
 */

#ifndef MEMORY_H_
#define MEMORY_H_
#include "Port.h"
class Memory {
public:

	typedef struct Param{
		int startAddress;
		int size; //bytes
		Port port;
	} Param, *ParamP;
	ParamP parameters;

	Memory(ParamP p);
	virtual ~Memory();
};

#endif /* MEMORY_H_ */
