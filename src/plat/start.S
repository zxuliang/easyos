#include <easyos.h>

.text
.align 2

.extern vectors_start
.extern vectors_end
.extern demo_thread_app

.global reset_handler
reset_handler:
	ldr	sp, =svc_stack_top
	bl	relocate_vectors
	msr	cpsr, #(IRQ_MODE|NO_INTRPT_MASK)
	ldr	sp, =irq_stack_top
	msr	cpsr, #SVC_MODE
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
