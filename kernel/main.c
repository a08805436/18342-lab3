/*
 * main.c: Kernel's entry point 
 *
 * Authors: Sridhar Srinivasan <sridhar1@andrew.cmu.edu>
 *          Ramya Bolla <rbolla@andrew.cmu.edu>
 *          Vinay Prasad <vinayp1@andrew.cmu.edu>
 */


#include <exports.h>

#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/interrupt.h>
#include <arm/timer.h>
#include "handlers.h"
#include "kernel.h"

/*
 * globals
 */
uint32_t global_data;
char irq_stack[IRQ_STACK_SIZE];
extern unsigned int lr_for_exit;
extern unsigned int sp_for_exit;

/* 
 * function that saves the sp and lr to which kernel's main has to return to
 * during the exit syscall handler.
 * @param: sp- stack pointer
 * @param: lr- link register (return address within uboot)
 * @return: void
 */
void save_lr_and_sp_for_exit(unsigned int lr, unsigned int sp)
{
	printf("\n saving lr and sp for exit");
	lr_for_exit = lr;
	sp_for_exit = sp;
}

/*
 * function that sets up the user stack
 * @param: argc- number of command line args
 * @param: argv- pointer to argument vector
 * @return: user mode stack pointer after setting up args on stack
 */
unsigned int *setup_user_stack(int argc, char *argv[])
{
	int i;
	unsigned int *user_stack_ptr = (unsigned int *)USER_STACK_BOTTOM;

	/*
	 * put a NULL at the bottom of the stack
	 */
	*(user_stack_ptr) = (unsigned int)0;
	user_stack_ptr--;

	/*
	 * setup the argument vector
	 */
	for(i = argc - 1; i >=0; i--) {
		*(user_stack_ptr) = (unsigned int)argv[i];
		user_stack_ptr--;
	}
	/*
	 * finally write argc
	 */
	*(user_stack_ptr) = (unsigned int)argc;
	return user_stack_ptr;
}

int kmain(int argc, char** argv, uint32_t table)
{
	app_startup(); /* bss is valid after this point */
	global_data = table;

	/* Add your code here */
	unsigned int *user_stack_ptr;
	
	/*
	 * set up the custom swi ad irq handler by hijacking the existing swi handling
	 * infrastructure
	 */
	if(install_handler((unsigned int *)SWI_VECTOR_ADDR, (void *)s_handler) < 0){
		printf("\n KERNEL MAIN: installation of custom SWI handler failed");
		return 0xbadc0de;
}
	
	if(install_handler((unsigned int *)IRQ_VECTOR_ADDR, (void *)i_handler) < 0){
		printf("\n KERNEL MAIN: installation of custom IRQ handler failed");
		return 0xbadc0de;
}

	printf("\n finidhed installing handler");	

	/*
	 * init the IRQ related registers
	 */
	init_irq_regs();	
	
	printf("\n finidhed initing irq regs");	
	/*
	 * setup IRQ stack
	 */
	setup_irq_stack(irq_stack + IRQ_STACK_SIZE - sizeof(long));
	printf("\n finidhed setting up irq stack");	

	/*
	 * init the timer driver
	 */
	init_timer_driver();
	printf("\n finidhed initing timer driver");	

	/*
	 * setup the user stack with command line args
	 */
	user_stack_ptr = setup_user_stack(argc, argv);
	printf("\n finished setting up user stack");	

	/*
	 * launch the user task
	 */
	launch_user_app(user_stack_ptr);

	/*
	 * this is the point we will return to during the exit syscall
	 * from here, we return to uboot
	 */
    return (get_kernel_r0()); 
	return 0;
}
