/*
 * kernelev.h
 *
 *  Created on: Sep 17, 2020
 *      Author: OS1
 */

#ifndef KERNELEV_H_
#define KERNELEV_H_

#include "event.h"
#include "kernsem.h"


class KernelEv { //
private:
	IVTNo brUlaza;
	KernelSem* eventSem;
	Event *myEvent;

public:
	KernelEv (IVTNo);
	~KernelEv ();
	void wait();
	void signal();
};

#endif /* KERNELEV_H_ */
