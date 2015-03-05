/*
 * critical.h
 *
 *  Created on: 2014-03-02
 *      Author: Jonah
 */

#ifndef CRITICAL_H_
#define CRITICAL_H_

#include "fingerprint.h"

//These are tasks to be fingerprinted
void critical_task(int count);
void preempt_task(int priority);

//These two tasks are used to save the values of callee saved
//registers onto the stack before fingerprinting and restore them
//after fingerprinting
extern void context_switch(long *reg_stack);
extern void context_restore(long *reg_stack);

#endif /* CRITICAL_H_ */
