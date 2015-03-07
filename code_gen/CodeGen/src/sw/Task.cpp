/*
 * Task.cpp
 *
 *  Created on: Feb 28, 2015
 *      Author: jonah
 */

#include "galloc.h"
#include <stdlib.h>
#include "Task.h"
Task::Task(){
	parameters = (ParamP) galloc((size_t)1,(size_t)sizeof(ParamP));
}
Task::Task(ParamP p) {
	parameters = p;
}

Task::~Task() {
	// TODO Auto-generated destructor stub
}

