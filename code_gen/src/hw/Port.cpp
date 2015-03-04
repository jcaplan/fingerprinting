/*
 * Port.cpp
 *
 *  Created on: Feb 28, 2015
 *      Author: jonah
 */

#include "Port.h"
#include <stdlib.h>

Port::Port(){
	bus = (Bus*)malloc(sizeof(Bus*));
	size = 0;
	startAddress = 0;
}
Port::Port(Bus *b, int size, int startAddress) {
	bus = b;
	this->size = size;
	this->startAddress = startAddress;
	// TODO Auto-generated constructor stub

}

Port::~Port() {
	// TODO Auto-generated destructor stub
}

