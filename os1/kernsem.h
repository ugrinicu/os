/*
 * kernelsem.h
 *
 *  Created on: Sep 15, 2020
 *      Author: OS1
 */

#ifndef KERNSEM_H_
#define KERNSEM_H_
class Scheduler;
class PCB;

extern Scheduler *scheduler;
extern volatile PCB *running;
typedef unsigned int Time;
class KernelList{
public:
	PCB* item;
	Time timeLeft;
	int returnValue;
	KernelList* next;
};

class KernelSem;

class StaticKSList{
public:
	KernelSem * item;
	StaticKSList * next;
};

class KernelSem {

friend class Semaphore;

private:
	friend void interrupt timer(...);
	int value;
	KernelList *waiting;
	void insert(KernelList*);

public:
	static StaticKSList * allLists;
	KernelSem (int init);
	~KernelSem ();
	int wait (Time);
	void signal ();
	int val () const;
};



#endif /* KERNSEM_H_ */
