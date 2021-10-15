#include "thread.h"
#include "Lock.h"
#include "SCHEDULE.H"
#include <iostream.h>
#include <dos.h>
#include "PCB.h"
#include "NewDel.h"

volatile static ID idCNT = 1;
volatile int zahtevana_promena_konteksta = 0;
volatile PCB* running = 0;

void startThread() {
	running->myThread->run();
	exitThread();
}


void exitThread(){
	INT_LOCK
	Thread::threadArray[Thread::getRunningId()] = 0;
	syncPrintf("THREAD #%d HAS FINISHED\n", Thread::getRunningId());
	running->zavrsio = 1;
	ThreadList* current = running->myThread->waiting;
	ThreadList* beforeCurr = 0;
	while(current){
		if(current->item && current->item->myPCB){
			current->item->myPCB->waiting = 0;
			Scheduler::put(current->item->myPCB);
		}
		beforeCurr = current;
		current = current->next;
		if(beforeCurr)delete beforeCurr;
	}
	if(beforeCurr)delete beforeCurr;
	INT_UNLOCK
	dispatch();

}

Thread::Thread(StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice) {
	INT_LOCK
	waiting = 0;

	this->myID = idCNT++;
	Thread::threadArray[myID] = this;
	myPCB = new PCB(this, stackSize);
	myPCB->kvant = timeSlice;
	myPCB->waiting=0;
}

void Thread::start() {
	INT_LOCK
	if(myPCB)Scheduler::put(this->myPCB);
}

void Thread::waitToComplete() {
	INT_LOCK
	if(running == myPCB)return;
	if (myPCB && this->myPCB->zavrsio == 1)return;

	running->waiting = 1;
	ThreadList* current = waiting;
	if(running->myThread && waiting == 0){
		waiting = new ThreadList();
		waiting->item = running->myThread;
		waiting->next = 0;
	}
	else if (running->myThread) {
		while (current->next) current = current->next;
		current->next = new ThreadList();
		current->next->item = running->myThread;
		current->next->next = 0;
	}
	INT_UNLOCK
	dispatch();
}

Thread::~Thread(){
	//syncPrintf("Thread #%d is waiting on Thread #%d \n",running->myThread->myID,this->myID);
	waitToComplete();
	//delete myPCB;
}

ID Thread::getRunningId(){
	INT_LOCK
	return running->myThread->myID;
}

Thread::Thread(){
	INT_LOCK
	waiting = 0;

	this->myID = idCNT++ %200;
	myPCB = new PCB(this, defaultStackSize);
	myPCB->kvant = defaultTimeSlice;
	myPCB->waiting=0;
}

ID Thread::getId(){
	return this->myID;
}

Thread* Thread::getThreadById(ID id){
	INT_LOCK
	return Thread::threadArray[id];
}
