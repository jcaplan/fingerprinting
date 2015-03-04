/*
 * Fprint.h
 *
 *  Created on: Feb 28, 2015
 *      Author: jonah
 */

#ifndef SRC_FPRINT_H_
#define SRC_FPRINT_H_

class Fprint {
public:

	int stackDepth;
	int blockSize;

	Fprint(int stackDepth, int blockSize);
	virtual ~Fprint();
};

#endif /* SRC_FPRINT_H_ */
