/*
 * event.cpp
 *
 *  Created on: Sep 17, 2020
 *      Author: OS1
 */
#include "event.h"
#include "thread.h"
#include "kernelev.h"
#include "lock.h"
#include <dos.h>
#include "NewDel.h"
extern volatile PCB* running;
Event::Event(IVTNo ivtNo){
	this->owner = running;
	this->myImpl = new KernelEv(ivtNo);
}

Event::~Event(){
	INT_LOCK
	delete myImpl;
	owner = 0;
}


void Event::wait(){
	if(running != owner)return;
	this->myImpl->wait();
}

void Event::signal(){
	this->myImpl->signal();
}


IVTEntry* IVTEntry::entries[256];

IVTEntry::IVTEntry(IVTNo num,
				InterruptRoutine introutine){
	INT_LOCK
	numOfEntry = num;
	this->entries[num] = this;
	newRoutine = introutine;
#ifndef BCC_BLOCK_IGNORE
	oldRoutine = getvect(num);
	setvect(num, introutine);
#endif
}

void IVTEntry::signalEvent(){
	if(entries[numOfEntry] != 0 && entries[numOfEntry]->myEvent != 0) myEvent->signal();
}

void IVTEntry::setEvent(KernelEv* ev){
	this->myEvent = ev;
}

IVTEntry::~IVTEntry(){
	INT_LOCK;
#ifndef BCC_BLOCK_IGNORE
	setvect(numOfEntry, oldRoutine);
	entries[numOfEntry]=0;
	oldRoutine();
#endif
	//delete myEvent;
}
