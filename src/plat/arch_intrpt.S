#include <easyplat.h>

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

