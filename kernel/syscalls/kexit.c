/*
 * kexit.c: Contains the implementaton of the exit syscall handler
 *
 * Authors: Sridhar Srinivasan <sridhar1@andrew.cmu.edu>
 *          Ramya Bolla <rbolla@andrew.cmu.edu>
 *          Vinay Prasad <vinayp1@andrew.cmu.edu>
 */

/*
 * globals
 */
unsigned int lr_for_exit;
unsigned int sp_for_exit;

/*
 * prototypes
 */
void kexit_asm(int status, unsigned int lr_for_exit, unsigned int sp_for_exit);

/*
 * kexit: implementation of the exit handler
 * @param: exit status
 * @return : does not return
 */
void kexit(int status)
{
	kexit_asm(status, lr_for_exit, sp_for_exit);
}
