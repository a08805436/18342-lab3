/*
 * timer.c: Implemetation of the timer driver 
 *
 * Authors: Sridhar Srinivasan <sridhar1@andrew.cmu.edu>
 *          Ramya Bolla <rbolla@andrew.cmu.edu>
 *          Vinay Prasad <vinayp1@andrew.cmu.edu>
 */

#include <arm/timer.h>
#include <arm/reg.h>
#include <types.h>
#include <exports.h>

#define TIMER_FREQ_FACTOR 100

/*
 * globals
 */
volatile unsigned long num_ticks;

void init_timer_driver(void)
{
	uint32_t oscr_10ms = 0;
	uint32_t oier_reg;

	/*
	 * init the oscr with 0
	 */
	reg_write(OSTMR_OSCR_ADDR, 0x0);
		
	/*
	 * calculate the oscr value for a 10 ms duration
	 */
	oscr_10ms = OSTMR_FREQ/TIMER_FREQ_FACTOR;

	/*
	 * init the osmr0 reg to oscr10_ms
	 */
	reg_write(OSTMR_OSMR_ADDR(0), oscr_10ms);
	
	/*
	 * activate the osmr0 bit in oier
	 */
	oier_reg = reg_read(OSTMR_OIER_ADDR);
	oier_reg |= OSTMR_OIER_E0;
	reg_write(OSTMR_OIER_ADDR, oier_reg);
	
	/*
	 * all set!
	 */
	return;
}

void handle_timer_irq(void)
{
	/*
	 * increment the numticks
	 */
	num_ticks++;

	/*
	 * reset the OSCR
	 */
	reg_write(OSTMR_OSCR_ADDR, 0x0);
	
	return;
}
