/*
 * basicmath.h
 *
 *  Created on: 2014-02-28
 *      Author: Jonah
 */

#ifndef BASICMATH_H_
#define BASICMATH_H_

typedef struct BasicmathSol{
	int num_solution;
	double x[3];
}BasicmathSol;


void basicmath_small(int* loopvalues, BasicmathSol* sol);


#endif /* BASICMATH_H_ */
