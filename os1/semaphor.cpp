/*
 * semaphor.cpp
 *
 *  Created on: Sep 15, 2020
 *      Author: OS1
 */

#include "semaphor.h"
#include "lock.h"
#include "kernsem.h"
#include "NewDel.h"

Semaphore::Semaphore(int init){
	lock();
	myImpl = new KernelSem(init);
	unlock();
}


Semaphore::~Semaphore(){
	INT_LOCK
	delete myImpl;
}


int Semaphore::wait(Time maxTimeToWait){
	return myImpl->wait(maxTimeToWait);
}


int Semaphore::signal(int n){
	INT_LOCK
	int ret;
	if(n<0)return n;
	if(n == 0)n=1;
	int i;

	if(myImpl->value < 0) ret = 0;
	else if(n > myImpl->value)ret = myImpl->value;
	else ret = n;

	for(i=0; i < n; i++){
		myImpl->signal();
	}
	return n;

}

int Semaphore::val() const {
	return myImpl->val();
}
