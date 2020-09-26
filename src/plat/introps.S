#include <easyos.h>

.text
.align 2
.global irq_lock_save
.type irq_lock_save,%function
irq_lock_save:
	mrs	r0, cpsr
	orr r1, r0, #NO_INTRPT_MASK
	msr cpsr, r1
	bx lr

.global irq_unlock_restore
.type irq_unlock_restore, %function
irq_unlock_restore:
	msr cpsr, r0
	bx	lr

.global irq_enable
.type irq_enable, %function
irq_enable:
	mrs r0, cpsr
	bic r0, r0, #NO_INTRPT_MASK
	msr cpsr, r0
	bx lr

.global irq_disable
.type irq_disable, %function
irq_disable:
	mrs r0, cpsr
	orr r0, r0, #NO_INTRPT_MASK
	msr cpsr, r0
	bx lr

/*---------------------isr-vector---------------------------*/
.extern do_irq_handle_ops
.global irq_handler
irq_handler:
#if 0
	sub lr, lr, #4
	stmfd sp!, {r0-r12, lr}
	bl do_irq_handle_ops
	ldmfd sp!, {r0-r12, pc}^		/* spsr->cpsr if pc as target */
#else
	sub	lr,	lr,	#4
	stmfd	sp!,	{lr}			/* save pc */
	stmfd	sp!,	{r0-r12, lr}		/* save interrupted-context */
	mrs	r0,	spsr			/* save interrupted-cpsr, that is store into SPSR */
	stmfd	sp!,	{r0}

	bl	do_irq_handle_ops

	ldmfd	sp!,	{r0}			/* restore spsr */
	msr	spsr,	r0
	ldmfd	sp!,	{r0-r12, lr, pc}^	/* restore all context, pc:as target,cpsr<--spsr  */
#endif

.global undef_handler
undef_handler:
	b .

.global swi_handler
swi_handler:
	b .

.global prefetch_abort_handler
prefetch_abort_handler:
	b .

.global	fiq_handler
fiq_handler:
	b .

.global	data_abort_handler
data_abort_handler:
	b .

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
