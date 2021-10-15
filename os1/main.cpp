#include <iostream.h>
#include <dos.h>
#include "event.h"
#include "SCHEDULE.H"
#include "Lock.h"
#include "kernsem.h"
#include "thread.h"
#include "PCB.h"
#include "timer.h"
#include <stdlib.h>
#include "NewDel.h"
unsigned oldTimerOFF, oldTimerSEG;
Idle* Thread::idleThread;
Thread* Thread::threadArray[200];
//StaticKSList* KernelSem::allLists;

void dispatch(){ // sinhrona promena konteksta
	zahtevana_promena_konteksta = 1;
	timer();
}

// postavlja novu prekidnu rutinu
void inic(){
	INT_LOCK

	zahtevana_promena_konteksta = 0;


	asm{
		cli
		push es
		push ax

		mov ax,0   //  ; inicijalizuje rutinu za tajmer
		mov es,ax

		mov ax, word ptr es:0022h //; pamti staru rutinu
		mov word ptr oldTimerSEG, ax
		mov ax, word ptr es:0020h
		mov word ptr oldTimerOFF, ax

		mov word ptr es:0022h, seg timer	 //postavlja
		mov word ptr es:0020h, offset timer //novu rutinu

		mov ax, oldTimerSEG	 //	postavlja staru rutinu
		mov word ptr es:0182h, ax //; na int 60h
		mov ax, oldTimerOFF
		mov word ptr es:0180h, ax

		pop ax
		pop es
		sti
	}


	//old_route = getvect(0x08);
	//setvect(0x08,timer);

	//Thread* thread = new Thread();
	//timer_tics_cnt = thread->myPCB->kvant;


	//running = thread->myPCB;

	//global_list->addPCB((PCB*)running);
	running = new PCB(0);
	Thread* mainThread = new Thread();
	free(mainThread->myPCB);
	running->myThread = mainThread;
	mainThread->myPCB = (PCB*)running;

	Thread::idleThread = new Idle();
	Thread::idleThread->myPCB->waiting = 1;
}

// vraca staru prekidnu rutinu
void restore(){
	INT_LOCK
	cout<<"BEGGINGINGINGIO"<<endl;
	delete []IVTEntry::entries;
	delete []Thread::threadArray;
	while(KernelSem::allLists){
		StaticKSList * temp = KernelSem::allLists;
		KernelSem::allLists = KernelSem::allLists->next;
		delete temp;
	}
	asm {
		cli
		push es
		push ax

		mov ax,0
		mov es,ax


		mov ax, word ptr oldTimerSEG
		mov word ptr es:0022h, ax
		mov ax, word ptr oldTimerOFF
		mov word ptr es:0020h, ax

		pop ax
		pop es
		sti
	}
	//delete running->myThread;
	delete running;
	delete Thread::idleThread->myPCB;
	//delete Thread::idleThread;

	cout<<"Alloc counter = "<<_allocCNT<<endl;
	cout<<"System restore"<<endl;


	//setvect(0x1C,old_route);
}


extern int userMain(int argc, char* argv[]);

int main(int argc, char** argv){
  inic();

  argc = 3;
  argv[1] = "60";
  argv[2] = "60";
  argv[3] = "2";
  userMain(argc,argv);

  restore();

  return 0;
}
