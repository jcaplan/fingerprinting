/*
 * Timer.h
 *
 *  Created on: Feb 28, 2015
 *      Author: jonah
 */

#ifndef SRC_TIMER_H_
#define SRC_TIMER_H_

class Timer {
public:
	int period;

	Timer(int per);
	virtual ~Timer();
};

#endif /* SRC_TIMER_H_ */
