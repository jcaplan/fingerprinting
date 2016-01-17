/*
 * gp.h
 *
 *  Created on: Oct 2, 2014
 *      Author: jonah.caplan
 */

#ifndef GP_H_
#define GP_H_

//get_gp() returns the value of gp register as a void*
void* get_gp(void);

//set_gp() gets the value of the Monitore core's gp
void set_gp(void *newGP);

//restore_gp() is called after set_gp() to return the gp register
//to its original value
void restore_gp(void);
#endif /* GP_H_ */
