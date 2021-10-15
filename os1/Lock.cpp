// File: Lock.cpp
#include "Lock.h"
#include <DOS.H>
#include <STDIO.H>
#include <STDARG.H>

void dispatch();
int syncPrintf(const char *format, ...)
{
	int res;
	va_list args;
	asm cli;
	va_start(args, format);
	res = vprintf(format, args);
	va_end(args);
	asm sti;
	return res;
}

volatile unsigned int lockFlag = 0;

INTlock::INTlock(int _lock):isLocked(0)
{
	if (_lock) lock();
}

INTlock::~INTlock()
{
	unlock();
}

void INTlock::lock() {
	if (isLocked) return;
	int tmp;
	#ifndef BCC_BLOCK_IGNORE
	asm {
		pushf
		pop tmp
		cli
	};
	#endif
	psw=tmp;
	isLocked=1;
}

void INTlock::unlock() {
	int tmp=psw;
	#ifndef BCC_BLOCK_IGNORE
	asm {
		push tmp
		popf
	};
	#endif
	isLocked=0;
}

void lock() {
	INT_LOCK
	lockFlag++;
}

void unlock() {
	INT_LOCK
	if (lockFlag>0) lockFlag--;

}
