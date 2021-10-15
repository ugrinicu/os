/*
 * kernelev.cpp
 *
 *  Created on: Sep 17, 2020
 *      Author: OS1
 */
#include "kernelev.h"
#include "kernsem.h"
#include "lock.h"
#include "NewDel.h"
void KernelEv::wait(){
	eventSem->wait(0);
}

void KernelEv::signal()  {
	INT_LOCK
	int x = eventSem->val();
	if(x < 0) x = -x;
	else if(x==0)x=1;
	else return;
	int i;
	for(i=0; i<x; i++)eventSem->signal();
}


KernelEv::~KernelEv(){
	INT_LOCK
	//delete eventSem;
	delete myEvent;
}

KernelEv::KernelEv(IVTNo nm){
	INT_LOCK
	eventSem = new KernelSem(0);
	IVTEntry::entries[nm]->setEvent(this);
}
