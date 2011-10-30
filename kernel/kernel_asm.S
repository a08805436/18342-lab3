@ kernel__asm.S: asm stuff for the kernel 
@ authors: Sridhar Srinivasan <sridhar1>
@          Ramya Bolla <rbolla>
@          Vinay Prasad <vinayp1>

	.file	"kernel_asm.S"
	.text
	.align 2	
	.global s_handler
	.global i_handler	
	.global launch_user_app  
	.global get_kernel_r0
	.global setup_irq_stack

@ custom s_handler implementation
s_handler:
	sub sp, sp, #4                @ make space for storing spsr
	stmfd sp!, {r0-r12, lr}       
	ldr r8, [sp, #60]             @ restore kernel's copy of r8
	mrs r2, spsr                  
	str r2, [sp, #56]             @ save spsr on the space created on stack
	mov r1, sp                    @ setup 2nd arg for C_SWI_Handler
	ldr r0, [lr, #-4] 
	bic r0, r0, #0xff000000       @ setup 1st arg for C_SWI_Handler
	bl C_SWI_Handler              @hand off control to C_SWI_Handler
	ldr r2, [sp, #56]
	msr spsr, r2                  @ restore spsr
	ldmfd sp!, {r0-r12, lr}       @unwind stack and return to caller 
	add sp, sp, #4
	movs pc, lr
	
@ custom i_handler implementation TODO: shoud this be re-entrant ?
i_handler:
	stmfd sp!, {r0-r12, lr}       
	bl C_IRQ_Handler              @hand off control to C_IRQ_Handler
	ldmfd sp!, {r0-r12, lr}       @unwind stack and return to caller 
	subs pc, lr, #4	


@utility function that launches a user task
launch_user_app:
	stmfd sp!, {r8}
	stmfd sp!, {r0}
	mov r0, lr
	add r1, sp, #8 
	bl save_lr_and_sp_for_exit    @ save the sp and lr for use in exit handler
	ldmfd sp!, {r0}               
	mov r3, #0x10
	mrs ip, cpsr                  @ change mode bits in CPSR to <flags>i_USER
	bic ip, ip, #0x9F
	orr ip, ip, r3
	msr cpsr, ip
	mov sp, r0
	ldr pc, =0xa0000000           @ move control to _start of user task

@utility method for getting the value of r0 register
get_kernel_r0:
	mov pc, lr

@utility method to setup the IRQ mode stack bottom to sp_irq
setup_irq_stack:
	mov r1, #0x12
	mrs ip, cpsr                  @ change mode bits in CPSR to <flags>_IRQ
	bic ip, ip, #0x1F
	orr ip, ip, r1
	msr cpsr, ip                  @switch to IRQ mode
	mov sp, r0                    @point sp_irq to allocated irq_stack
	mov r1, #0x13
	mrs ip, cpsr                  @ change mode bits in CPSR to <flags>_SVC
	bic ip, ip, #0x1F
	orr ip, ip, r1
	msr cpsr, ip                  @switch back to SVC mode
	mov pc, lr
	
