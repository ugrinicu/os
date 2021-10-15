/*
 * timer.cpp
 *
 *  Created on: Sep 14, 2020
 *      Author: OS1
 */

#include <iostream.h>
#include <dos.h>
#include "SCHEDULE.H"
#include "Lock.h"
#include "PCB.h"
#include "timer.h"
#include "kernsem.h"
#include "thread.h"
volatile unsigned tsp;
volatile unsigned tss;
volatile unsigned tbp;
volatile unsigned tcs;
static int idleBool = 0;
IntRoutine old_route = 0;
volatile int brojac = 10;
extern volatile int zahtevana_promena_konteksta;

void interrupt timer(...){
	//if(zahtevana_promena_konteksta)syncPrintf("\nPozvan iz dispatch-a\nBrojac = %d",brojac);
	if (!zahtevana_promena_konteksta) {
		brojac--;
		StaticKSList* currentSem = KernelSem::allLists;
		while(currentSem){
			KernelList *current = currentSem->item->waiting;
			KernelList *beforeCurr = 0;
			while(current){
				int changed = 0;
				if(current->timeLeft > 1)current->timeLeft--;

				else if(current->timeLeft == 1){
					if(beforeCurr){
						beforeCurr->next = current->next;
						changed = 1;
					}
					else {currentSem->item->waiting = current->next; changed = 1;}
					Scheduler::put(current->item);
					//syncPrintf("Thread #%d released from timer\n",current->item->myThread->getId());
					current->item->returnValue = 0;
					current->item->waiting = 0;
					if(currentSem->item->value<0)currentSem->item->value++;
				}

				if(!changed)beforeCurr = current;
				current = current->next;
			}
			currentSem = currentSem->next;
		}
	}

	if (brojac <= 0 || zahtevana_promena_konteksta) {
		if (lockFlag == 0){
			zahtevana_promena_konteksta = 0;
			asm {
				// cuva sp
				mov tsp, sp
				mov tss, ss
				mov tbp, bp
			}

			running->sp = tsp;
			running->ss = tss;
			running->bp = tbp;


			if(!running->zavrsio && !running->waiting) Scheduler::put((PCB*) running);

			running = Scheduler::get();
			if(running == 0){
				running = Thread::idleThread->myPCB;
			}
			tsp = running->sp;
			tss = running->ss;
			tbp = running->bp;
			if(tbp == 0 && running->myThread)syncPrintf("AAAAAAAAAAA @ Thread:%d",running->myThread->myID);
			else if (tbp == 0)syncPrintf("AAAAAAAAAAA @ from thread == 0");
			brojac = running->kvant;
			//syncPrintf("Brojac = %d\n",brojac);
			asm {
				mov sp, tsp   // restore sp
				mov ss, tss
				mov bp, tbp
			}
		}
	}

	// poziv stare prekidne rutine koja se
     // nalazila na 08h, a sad je na 60h
     // poziva se samo kada nije zahtevana promena
     // konteksta – tako se da se stara
     // rutina poziva samo kada je stvarno doslo do prekida
	if(!zahtevana_promena_konteksta) asm int 60h;

}
