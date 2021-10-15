// File: Lock.h
#ifndef H_LOCK_H_
#define H_LOCK_H_

int syncPrintf(const char *format, ...);

extern volatile unsigned int lockFlag;


#define INT_LOCK INTlock _intLock;
#define INT_UNLOCK _intLock.unlock();
#define INT_RELOCK _intLock.lock();

class INTlock {
	int psw;
	int isLocked;
public:
	INTlock(int lock=1);
	~INTlock();

	void lock();
	void unlock();
};

void lock();
void unlock();


#endif /* H_LOCK_H_ */
