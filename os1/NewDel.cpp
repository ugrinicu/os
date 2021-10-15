//File: NewDel.cpp
// Contains redefined operations new & delete mad threadsafe
#include <iostream.h>
#include <malloc.h>
#include <stdlib.h>
#include "NewDel.h"
#include "Lock.h"

volatile long int _allocCNT;

void* operator new(unsigned size) {
	INT_LOCK

	void* ret = malloc(size);

	if(ret == 0 && size > 0) {
		cout << "\nERROR: Out of memory!\n";
		//exit(69);
	}

	_allocCNT++;
  return ret;
}

void operator delete(void* loc) {
	INT_LOCK

	if(loc != 0) {
		free(loc);
		_allocCNT--;
	}
}
