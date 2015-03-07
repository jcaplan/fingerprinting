/*
 * Signal.cpp
 *
 *  Created on: Feb 28, 2015
 *      Author: jonah
 */

#include <stdlib.h>
#include "galloc.h"
#include "Wire.h"
Wire::Wire(){
	parameters = (Param*)galloc(1,sizeof(Param));
}
Wire::Wire(ParamP p) {
	parameters = p;
}

Wire::~Wire() {
	// TODO Auto-generated destructor stub
}

