// File: thread.h
#ifndef _thread_h_
#define _thread_h_

typedef unsigned long StackSize;
const StackSize defaultStackSize = 4096;
typedef unsigned int Time; // time, x 55ms
const Time defaultTimeSlice = 2; // default = 2*55ms
typedef int ID;
class PCB; // Kernel's implementation of a user's thread
class Thread;

extern volatile int zahtevana_promena_konteksta;

void exitThread();

void startThread();

class ThreadList {
public:
	Thread* item;
	ThreadList* next;
};

class Idle;


class Thread {
public:
	Thread (StackSize, Time);
	Thread ();
	static Thread* threadArray[200];
	void start();
	ThreadList* waiting;
	void waitToComplete();
	virtual ~Thread();
	ID getId();
	static ID getRunningId();
	static Thread * getThreadById(ID id);
	static Idle* idleThread;
	PCB* myPCB;
protected:
	friend class KernelSem;
	friend void interrupt timer(...);
	friend class PCB;
	friend void startThread();
	friend void dispatch();
	friend void exitThread();

	virtual void run() {}
private:

	ID myID;
};

class Idle: public Thread{
	void run(){
		while(1);
	}
};


void dispatch ();
#endif
