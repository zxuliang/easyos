#include <easyplat.h>

.text
.align 2


.global reset_handler
reset_handler:
	ldr	sp, =svc_stack_top
	bl	relocate_vectors
	msr	cpsr, #(IRQ_MODE|NO_INTRPT_MASK)
	ldr	sp, =irq_stack_top
	msr	cpsr, #SVC_MODE
	bl	bsp_bss_clear
	bl	main
	b .

.global relocate_vectors
relocate_vectors:
		ldr r0, =vectors_start
		ldr r1, =vectors_end
		mov r2, #0x0
copy_loop:
		ldr r3, [r0], #4
		str r3, [r2], #4
		cmp r0, r1
		ble copy_loop
		bx lr

/*---------------------isr-vector---------------------------*/
.global vectors_start, vectors_end
vectors_start:
	ldr pc, reset_handler_addr
	ldr pc, undef_handler_addr
	ldr pc, swi_handler_addr
	ldr pc, prefetch_abort_handler_addr
	ldr pc, data_abort_handler_addr
	b .
	ldr pc, irq_handler_addr
	ldr pc, fiq_handler_addr

	reset_handler_addr		: .word reset_handler
	undef_handler_addr		: .word OS_CPU_ARM_ExceptUndefInstrHndlr
	swi_handler_addr		: .word OS_CPU_ARM_ExceptSwiHndlr
	prefetch_abort_handler_addr	: .word OS_CPU_ARM_ExceptPrefetchAbortHndlr
	data_abort_handler_addr		: .word OS_CPU_ARM_ExceptDataAbortHndlr
	irq_handler_addr		: .word OS_CPU_ARM_ExceptIrqHndlr
	fiq_handler_addr		: .word OS_CPU_ARM_ExceptFiqHndlr
vectors_end:

