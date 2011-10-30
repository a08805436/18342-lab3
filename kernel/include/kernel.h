/*
 * kernel.h: Defines constants used within the kernel 
 * Authors: Sridhar Srinivasan <sridhar1@andrew.cmu.edu>
 *          Ramya Bolla <rbolla@andrew.cmu.edu>
 *          Vinay Prasad <vinayp1@andrew.cmu.edu>
 */

#ifndef KERNEL_H
#define KERNEL_H

/*
 * user task details 
 */
#define USER_STACK_BOTTOM 0xa2FFFFFC

/*
 * IRQ details
 */
#define IRQ_STACK_SIZE 4096

/*
 * Prototypes
 */
void launch_user_app(unsigned int *);
void s_handler(void);
void i_handler(void);
unsigned int get_kernel_r0(void);
void setup_irq_stack(char *);

#endif /* KERNEL_H */
