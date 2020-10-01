#include <easyos.h>

.extern do_irq_handle_ops
.extern intrpt_context_switch
.extern kthread_fatal_error

.text
.align 2
.global irq_lock_save
irq_lock_save:
	mrs	r0, cpsr
	orr	r1, r0, #NO_INTRPT_MASK
	msr 	cpsr, r1
	bx 	lr

.align 2
.global irq_unlock_restore
irq_unlock_restore:
	msr 	cpsr, r0
	bx	lr

.align 2
.global irq_enable
irq_enable:
	mrs r0, cpsr
	bic r0, r0, #NO_INTRPT_MASK
	msr cpsr, r0
	bx lr

.align 2
.global irq_disable
irq_disable:
	mrs r0, cpsr
	orr r0, r0, #NO_INTRPT_MASK
	msr cpsr, r0
	bx lr

.align 2
.global arch_wfi_idle
arch_wfi_idle:
	mov	r0, #0
	mcr	p15, 0, r0, c7, c0, 4
	bx	lr


/*---------------------isr-handler---------------------------*/

.align 2
.global irq_handler
irq_handler:
	sub	lr,	lr,	#4
	stmfd	sp!,	{r0-r12, lr}
	bl	do_irq_handle_ops

	ldr	r0,	=intrpt_context_switch
	ldr	r1,	[r0]
	mov	r2,	#0
	cmp	r1,	#0
	strne	r2,	[r0]
	bne	irq_ctx_switch
	ldmfd	sp!,	{r0-r12, pc}^			/* r0-r3,r12, pc, spsr_irq->cpsr_svc */

irq_ctx_switch:
	ldmfd	sp!,	{r0-r12, lr}			/* r0-r12, lr */
	stmfd	sp!,	{r0-r3}				/* save r0-r3 in irq_stack */
	mov	r0,	sp				/* r0->sp_irq */
	add	sp,	sp,	#16			/* now sp_irq return to origin pos */
	mov	r1,	lr				/* r1<--lr_irq(pc_svc) */
	mrs	r2,	spsr				/* spsr_irq<-->cpsr_svc */
	mov	r3,	#SVC_MODE|NO_INTRPT_MASK
	msr	cpsr,	r3				/* switch to svc mode with irq disabled */
	stmfd	sp!,	{r1}				/* save pc_svc */
	stmfd	sp!,	{r4-r12, lr}			/* save r4-r12_svc, lr_svc */
	ldmfd	r0!,	{r4-r7}				/* get saved r0-r3 from irq_stack */
	stmfd	sp!,	{r4-r7}				/* store r0-r3_svc */
	stmfd	sp!,	{r2}				/* store cpsr_svc */
	mrs	r2,	spsr				/* read spsr_svc */
	stmfd	sp!,	{r2}				/* save spsr_svc */
	ldr	r3,	=current
	ldr	r3,	[r3]
	str	sp,	[r3]				/* save sp->tsp of current */

	/*
	 * Note: calling function will corrupted r0-r3
	 * caller need to save r0-r3 if needed.
	 * find next task, called with irq-disabled
	 * stack will auto restore after calling.
	*/
	bl	mico_os_find_next

	ldr	r3,	=current
	ldr	r3,	[r3]
	ldr	sp,	[r3]

	ldmfd	sp!,	{r0}				/* spsr_svc */
	msr	spsr,	r0
	ldmfd	sp!,	{r0}				/* cpsr_svc */
	msr	cpsr,	r0
	ldmfd	sp!,	{r0-r12, lr, pc}		/* r0-r12, lr, pc; run new task */

.align 2
.global undef_handler
undef_handler:
	b .

.align 2
.global swi_handler
swi_handler:
	b .

.align 2
.global prefetch_abort_handler
prefetch_abort_handler:
	b .

.align 2
.global	fiq_handler
fiq_handler:
	b .

.align 2
.global	data_abort_handler
data_abort_handler:
	b .

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
	undef_handler_addr		: .word undef_handler
	swi_handler_addr		: .word swi_handler
	prefetch_abort_handler_addr	: .word prefetch_abort_handler
	data_abort_handler_addr		: .word data_abort_handler
	irq_handler_addr		: .word irq_handler
	fiq_handler_addr		: .word fiq_handler
vectors_end:
