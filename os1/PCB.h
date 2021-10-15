/*
 * PCB.h
 *
 *  Created on: Sep 14, 2020
 *      Author: OS1
 */

#ifndef PCB_H_
#define PCB_H_

class Thread;

typedef unsigned long StackSize;


class PCB{
public:
	int returnValue;
	unsigned *st;
	unsigned bp;
	unsigned sp;
	unsigned ss;
	unsigned cs;
	unsigned ip;
	unsigned zavrsio;
	unsigned waiting;
	int kvant;
	Thread* myThread;
	PCB(Thread* t, StackSize);
	PCB(void (*body)());
	~PCB();
};

extern volatile PCB* running;

#endif /* PCB_H_ */
