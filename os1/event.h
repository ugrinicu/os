// File: event.h
#ifndef _event_h_
#define _event_h_

typedef unsigned char IVTNo;
typedef void interrupt (*InterruptRoutine) (...);

class KernelEv;
class PCB;
class IVTEntry;

class IVTEntry {
public:
	static IVTEntry* entries[256];
	int numOfEntry;
	KernelEv *myEvent;
	InterruptRoutine oldRoutine;
	InterruptRoutine newRoutine;
	IVTEntry(IVTNo, InterruptRoutine);
	void signalEvent();
	void setEvent(KernelEv*);
	~IVTEntry();
};

class Event {
public:
	Event (IVTNo ivtNo);
	~Event ();
	void wait ();

protected:
	friend class KernelEv;
	void signal(); // can call KernelEv

private:
	volatile PCB* owner;
	KernelEv* myImpl;
};

#define PREPAREENTRY(_ivtNo, _callOld)\
	void interrupt intr##_ivtNo(...);\
	IVTEntry ivte##_ivtNo(_ivtNo, intr##_ivtNo);\
	void interrupt intr##_ivtNo(...) {\
		ivte##_ivtNo.signalEvent();\
		if(_callOld)ivte##_ivtNo.oldRoutine();\
	}


#endif
