/*
 * exit.S: exit sycall wrapper
 * Author:  Sridhar  Srinivasan <sridhar1@andrew.cmu.edu>
 *         Vinay Prasad <vinayp1@andrew.cmu.edu>
 *         Ramya Bolla <rbolla@andrew.cmu.edu>
 * 
 */

#include <asm.h>
#include <bits/swi.h>

	.file "exit.S"
FUNC(exit)
exit:
	swi EXIT_SWI
