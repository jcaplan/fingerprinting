/*
 * Port.h
 *
 *  Created on: Feb 28, 2015
 *      Author: jonah
 */

class Port;

#ifndef SRC_PORT_H_
#define SRC_PORT_H_

#include "Bus.h"
#include <stddef.h>
class Port {
public:
	Bus *bus;
	int startAddress;
	int size;

	Port();
	Port(Bus *b, int size, int startAddress);
	virtual ~Port();
};

#endif /* SRC_PORT_H_ */
