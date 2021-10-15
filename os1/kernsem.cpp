/*
 * kernelsem.cpp
 *
 *  Created on: Sep 15, 2020
 *      Author: OS1
 */

#include "kernsem.h"
#include "schedule.h"
#include "thread.h"
#include "pcb.h"
#include "lock.h"
#include "NewDel.h"
void dispatch();

StaticKSList* KernelSem::allLists = 0;

void KernelSem::insert(KernelList *k){
	lock();
	KernelList * curr = waiting;
	if(waiting){
		while(curr->next)curr=curr->next;
		curr->next = k;
	}
	else {
		waiting = k;
	}
	unlock();
}

int KernelSem::val() const {
	return value;
}

int KernelSem::wait(Time time){
	INT_LOCK
	running->returnValue = 1;
	if(value<=0){

		running->waiting = 1;

		KernelList* k = new KernelList();
		k->item = (PCB *) running;
		k->timeLeft = time;
		k->next = 0;
		k->returnValue = 1;
		insert(k);
		INT_UNLOCK
		dispatch();
		delete k;
		//syncPrintf("Thread #%d is returning %d \n",running->myThread->myID,running->returnValue);
		return running->returnValue;
	}
	else { value--; return running->returnValue; }
}


void KernelSem::signal(){
	INT_LOCK
	value++;
	if(waiting){
		KernelList *k = waiting;
		waiting = waiting->next;
		k->item->waiting = 0;
		Scheduler::put(k->item);
	}
}

KernelSem::KernelSem(int init){
	waiting = 0;
	value = init;
	StaticKSList *k = allLists;
	allLists = new StaticKSList();
	allLists->item = this;
	allLists->next = k;
}

KernelSem::~KernelSem(){
	INT_LOCK

	KernelList* temp = 0;
	while (waiting){
		temp = waiting;
		waiting = waiting->next;
		if(temp)delete temp;
	}

	StaticKSList *x = allLists;
	StaticKSList* beforeX = 0;
	while(x){
		if(this == x->item){
			if(beforeX){
				beforeX->next = x->next;
			}
			delete x;
			break;
		}
		beforeX = x;
		x = x->next;
	}
	/*
	while(waiting){
		KernelList* k = waiting;
		waiting->item->waiting = 0;
		waiting->returnValue = 0;
		//Scheduler::put(waiting->item);
		waiting = waiting->next;
		delete k;
	}*/
}
