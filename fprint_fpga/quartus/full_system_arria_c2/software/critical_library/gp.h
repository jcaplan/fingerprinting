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

//set_gp() gets the value of the Monitore core's gp,
//assumed to be at address 0x3000000, and then sets
//the gp register of the core that calls set_gp() to
//the Monitor core's gp value
void set_gp(void);

//restore_gp() is called after set_gp() to return the gp register
//to its original value
void restore_gp(void);
#endif /* GP_H_ */
