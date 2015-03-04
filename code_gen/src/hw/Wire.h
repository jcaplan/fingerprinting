/*
 * Signal.h
 *
 *  Created on: Feb 28, 2015
 *      Author: jonah
 */

#ifndef SRC_SIGNAL_H_
#define SRC_SIGNAL_H_
#include "Port.h"

class Wire {
public:
	typedef struct Param{
	Port *slavePort;
	Port *masterPort;
	int width;
	}Param, *ParamP;
	ParamP parameters;

	Wire();
	Wire(ParamP p);
	virtual ~Wire();
};

#endif /* SRC_SIGNAL_H_ */
