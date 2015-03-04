/*
 * Application.h
 *
 *  Created on: Feb 28, 2015
 *      Author: jonah
 */

#ifndef SRC_SW_APPLICATION_H_
#define SRC_SW_APPLICATION_H_
#include "Task.h"

class Application {
public:

	typedef struct TaskList{
		Task value;
		Task next;
	}TaskList, *TaskListP;

	TaskList taskList;
	Application();
	virtual ~Application();
};

#endif /* SRC_SW_APPLICATION_H_ */
