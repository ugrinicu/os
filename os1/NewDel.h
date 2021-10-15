/*
 * NewDel.h
 *
 *  Created on: Sep 22, 2020
 *      Author: OS1
 */

#ifndef NEWDEL_H_
#define NEWDEL_H_

extern volatile long int _allocCNT;

void* operator new(unsigned size);

void operator delete(void* loc);

#endif /* NEWDEL_H_ */
