/*
 * Task.cpp
 *
 *  Created on: Feb 28, 2015
 *      Author: jonah
 */

#include "Task.h"
#include <stdlib.h>
Task::Task(){
	parameters = (ParamP) malloc(sizeof(ParamP));
}
Task::Task(ParamP p) {
	parameters = p;
}

Task::~Task() {
	// TODO Auto-generated destructor stub
}

