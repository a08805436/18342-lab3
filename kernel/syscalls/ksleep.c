/*
 * ksleep.c: Implementation of the sleep syscall 
 * Authors: Sridhar Srinivasan <sridhar1@andrew.cmu.edu>
 *          Ramya Bolla <rbolla@andrew.cmu.edu>
 *          Vinay Prasad <vinayp1@andrew.cmu.edu>
 *
 */

#include <exports.h>
#include <bits/errno.h>
#include <bits/fileno.h>

#define TIMER_INTR_INTERVAL 10

/*
 * globals
 */
extern volatile unsigned long num_ticks;

/*
 * implementation of the sleep syscall
 * @param: millis - number of milliseconds to sleep  
 * @return void 
 */
void ksleep(unsigned long millis)
{
	unsigned long sleep_till_ticks;
//	printf("\n inside sleep. numticks: %ld", num_ticks);	
	/*
	 * validate the millis arg
	 */
	if(millis == 0) {
		return;
	}

	/*
	 * calculate the number of ticks that should have
	 * elapsed before we wake up
	 */
	sleep_till_ticks = num_ticks + (millis/TIMER_INTR_INTERVAL);
//	printf("\n sleep till ticks is %ld", sleep_till_ticks);
	while(num_ticks < sleep_till_ticks);

	/*
	 * time to wake up!
	 */
//	printf("\n returning from sleep");
	return;
}
