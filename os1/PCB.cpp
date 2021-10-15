/*
 * PCB.cpp
 *
 *  Created on: Sep 14, 2020
 *      Author: OS1
 */
#include "PCB.h"
#include "thread.h"
#include "Lock.h"
#include "SCHEDULE.H"
#include <iostream.h>
#include <dos.h>
#include "NewDel.h"

PCB::PCB(Thread* thr, StackSize stackSize = defaultStackSize){
	myThread = thr;
	st = new unsigned[stackSize];
	st[stackSize - 1] = 0x200;
	#ifndef BCC_BLOCK_IGNORE
	st[stackSize - 2] = FP_SEG(startThread);
	st[stackSize - 3] = FP_OFF(startThread);
	this->sp = FP_OFF(st + stackSize - 12);
	this->ss = FP_SEG(st + stackSize - 12);
	this->bp = FP_OFF(st + stackSize - 12);
	#endif

	this->zavrsio = 0;
	if(!bp){
		cout<<"Thread #"<<thr->myID<<" HAS BP 0";
		asm cli;
	}
}

PCB::~PCB(){
	delete [] st;
	myThread = 0;
	zavrsio = 0;
	waiting = 0;
	bp = 0;
}

PCB::PCB(void (*body)()){
	StackSize stackSize = defaultStackSize;
	st = new unsigned[stackSize];
	st[stackSize - 1] = 0x200;
	#ifndef BCC_BLOCK_IGNORE
	st[stackSize - 2] = FP_SEG(body);
	st[stackSize - 3] = FP_OFF(body);
	this->sp = FP_OFF(st + stackSize - 12);
	this->ss = FP_SEG(st + stackSize - 12);
	this->bp = FP_OFF(st + stackSize - 12);
	#endif
	this->zavrsio = 0;
	this->myThread = 0;
	if(!bp){
		cout<<"Idle or main thread HAS BP 0";
		asm cli;
	}
}
