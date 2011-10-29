/*
 * handlers.c: wire-in and hijacking code for SWI and IRQ handlers
 *
 * Authors: Sridhar Srinivasan <sridhar1@andrew.cmu.edu>
 *          Ramya Bolla <rbolla@andrew.cmu.edu>
 *          Vinay Prasad <vinayp1@andrew.cmu.edu>
 */
#include "handlers.h"
#include <exports.h>
#include <bits/swi.h>

/*
 * globals
 */
unsigned int def_swi_handler_inst1, def_swi_handler_inst2;
unsigned int def_irq_handler_inst1, def_irq_handler_inst2;
unsigned int *def_swi_handler_loc;
unsigned int *def_irq_handler_loc;

 /*
 * function that installs the custom handler by hijacking the first 2 
 * instructions of the default handler
 * @param: void
 * @return: 0 on success, < 0 on failure
 */
int install_handler(unsigned int *vector_addr, void *handler_addr)
{
	unsigned int vector_inst;
	unsigned int offset;
	unsigned int *def_handler_loc;

	/*
	 * validate the vector address
	 */
	printf("\n inside install handler, vector addr is %p handler_addr is %p\n", vector_addr, handler_addr);
	if((vector_addr != (unsigned int *)SWI_VECTOR_ADDR) &&
	   (vector_addr != (unsigned int *)IRQ_VECTOR_ADDR)) {
		printf("Invalid vector address passed to install_handler\n");
		return -1;
	}
	vector_inst = *vector_addr;
	offset = vector_inst & LDR_INST_OFFSET;
	/*
	 * validate the LDR instruction stored at SWI vector
	 */
	if((vector_inst & ~LDR_INST_OFFSET) != LDR_VALID_FORMAT) {
		printf("\nInvalid LDR instruction at vector,can't install handler");
		return -1;
	}
	
	printf("\n install handler offset is %x", offset);
	/*
	 * fetch the offset from the LDR inst and go to the default handler
	 */
	def_handler_loc = (unsigned int *)(*(unsigned int *)((char *)vector_addr + 
												 ACTUAL_PC_OFFSET + offset));

	printf("\n install handler loc is %p", def_handler_loc);
	/*
	 * save the existing instructions in the default handler
	 */
	switch ((unsigned int)vector_addr) {
		case SWI_VECTOR_ADDR:
		 	printf("\n install handler inside SWI case");
			def_swi_handler_inst1 = *def_handler_loc;
			def_swi_handler_inst2 = *(def_handler_loc +1);
			def_swi_handler_loc = def_handler_loc;
		 	printf("\n install handler def swi handler loc is %p", def_swi_handler_loc);
			break;
		case IRQ_VECTOR_ADDR:
		 	printf("\n install handler inside IRQ case");
			def_irq_handler_inst1 = *def_handler_loc;
			def_irq_handler_inst2 = *(def_handler_loc +1);
			def_irq_handler_loc = def_handler_loc;
			break;		
		}
	 	
	/*
	 * overwrite the 1st 2 instructions of the default handler
	 */
	*def_handler_loc = (unsigned int)CUSTOM_HANDLER_INST1;
	*(def_handler_loc + 1) = (unsigned int)handler_addr;

	/*
	 * all set!
	 */
	printf("\n install handler returning 0");
	return 0;
}

/*
 * implementation of the C_SWI_Handler
 * @param: swi_num- swi number
 * @param: sp- stack pointer that points to block of user registers
 * @return: return value form swi on success, -1 on failure (swi num not 
            supported)
 */
int C_SWI_Handler(int swi_num, unsigned int *sp)
{
	ssize_t retval = -1;
	unsigned int r0, r1, r2;
	printf("\n inside C_SWI_Handler: swi num is %x", swi_num);
	switch(swi_num) {
		case READ_SWI:
			r0 = *sp;
			r1 = *(sp + 1);
			r2 = *(sp + 2);
			retval = kread((int)r0, (void *)r1, (size_t)r2);
			break;
		case WRITE_SWI:
			r0 = *sp;
			r1 = *(sp + 1);
			r2 = *(sp + 2);
			retval = kwrite((int)r0, (const void *)r1, (size_t)r2);
			break;
		case EXIT_SWI:
			/*
			 * restore the original s_handler and i_handler before dying 
			 */
			*def_swi_handler_loc = def_swi_handler_inst1;
			*(def_swi_handler_loc + 1) = def_swi_handler_inst2;
/*
			*def_irq_handler_loc = def_irq_handler_inst1;
			*(def_irq_handler_loc + 1) = def_irq_handler_inst2;
*/
			r0 = *sp;
			kexit((int)r0);
			break;
		default:
		    printf("\n C_SWI_Handler:invalid SWI call,bail out with error\n");
			/*
			 * invalid swi call, bail out after marking failure 
			 * restore the original s_handler before dying 
			 */
			*def_swi_handler_loc = def_swi_handler_inst1;
			*(def_swi_handler_loc + 1) = def_swi_handler_inst2;
/*
			*def_irq_handler_loc = def_irq_handler_inst1;
			*(def_irq_handler_loc + 1) = def_irq_handler_inst2;
*/
			kexit(0xbadc0de);
	}
	return retval;
}

int C_IRQ_Handler(int irq_num, unsigned int *sp) 
{
	printf("\n inside C_IRQ_Handler: irq num is %d", irq_num);
	return 0;
}


