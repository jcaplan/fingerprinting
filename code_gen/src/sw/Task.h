/*
 * Task.h
 *
 *  Created on: Feb 28, 2015
 *      Author: jonah
 */

#ifndef SRC_TASK_H_
#define SRC_TASK_H_

class Task {
public:
	typedef struct Param{
		int periodMS;
		int startTime;
		bool critical;
		char* computationFile;
		int timeTriggered;
		int startConditions;
	}Param, *ParamP;

	ParamP parameters;

	Task();
	Task(ParamP p);
	virtual ~Task();
};

#endif /* SRC_TASK_H_ */
