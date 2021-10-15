#ifndef _semaphor_h_
#define _semaphor_h_

typedef unsigned int Time;
class KernelSem;

class Semaphore {
	public:
	Semaphore (int init=1);
	~Semaphore ();
	int wait (Time maxTimeToWait);
	int signal(int n=0);
	int val () const;  // Returns the current value of the semaphore
	
	private:
	KernelSem* myImpl;
};
#endif
