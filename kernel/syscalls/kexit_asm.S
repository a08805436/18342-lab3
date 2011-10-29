@ "kexit_asm.S" contains some asm utils for the exit handler
@ authors: Sridhar Srinivasan <sridhar1>
@          Ramya Bolla <rbolla>
@          Vinay Prasad <vinayp1>

	.file	"kexit_asm.S"
	.text
	.align 2
	.global kexit_asm

@utility function used by exit handler to simulate a return from kernel main
kexit_asm:
	mov sp, r2
	mov pc, r1
