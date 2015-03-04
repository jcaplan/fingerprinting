/*
 * BusBridge.h
 *
 *  Created on: Feb 28, 2015
 *      Author: jonah
 */

#ifndef BUSBRIDGE_H_
#define BUSBRIDGE_H_
#include "Port.h"

class BusBridge {
public:

	typedef struct PortList{
		Port *value;
		Port *next;
	} PortList, *PortListP;

	PortList slavePorts;
	Port masterPort;

	BusBridge();
	virtual ~BusBridge();
};

#endif /* BUSBRIDGE_H_ */
