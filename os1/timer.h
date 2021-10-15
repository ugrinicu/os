/*
 * timer.h
 *
 *  Created on: Sep 14, 2020
 *      Author: OS1
 */

#ifndef TIMER_H_
#define TIMER_H_


typedef void interrupt (*IntRoutine) (...);
extern IntRoutine old_route;
extern volatile int brojac;
void interrupt timer(...);

#endif /* TIMER_H_ */
