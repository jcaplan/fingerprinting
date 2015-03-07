/*
 * Bus.h
 *
 *  Created on: Feb 28, 2015
 *      Author: jonah
 */
class Bus;

#ifndef BUS_H_
#define BUS_H_

#include "Port.h"

class Bus {
public:
	typedef struct Param{
	Port *slavePort;
	Port *masterPort;
	}Param, *ParamP;

	ParamP parameters;

	Bus(ParamP p);
	virtual ~Bus();
};

#endif /* BUS_H_ */
