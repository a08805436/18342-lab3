/*
 * ktime.c: Implementation of the time syscall 
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
 * implementation of the time syscall
 * @param: void 
 * @return unsigned long - time in milliseconds since bootup 
 */
unsigned long ktime()
{
	return (num_ticks * TIMER_INTR_INTERVAL);
}
