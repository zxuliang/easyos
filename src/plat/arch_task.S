
.text
.align 2
.extern current
.global mico_os_start
/* void mico_os_start(void) */
mico_os_start:
	bl	mico_os_find_next
	ldr	r0, =current
	ldr	r0, [r0]
	ldr	sp, [r0]
	ldmfd	sp!,{r0}
	msr	spsr, r0
	ldmfd	sp!,{r0}
	msr	cpsr, r0
	ldmfd	sp!,{r0-r12, lr, pc}

.align 2
.global mico_os_ctx_switch
/* void mico_os_ctx_switch(void) */
mico_os_ctx_switch:
	stmfd	sp!, {lr}
	stmfd	sp!, {r0-r12, lr}
	mrs 	r0, cpsr
	stmfd	sp!, {r0}
	mrs	r0, spsr
	stmfd	sp!, {r0}
	ldr	r0, =current
	ldr	r0, [r0]
	str	sp, [r0]

	bl	mico_os_find_next

	ldr	r0, =current
	ldr	r0, [r0]
	ldr	sp, [r0]
	ldmfd	sp!,{r0}
	msr	spsr, r0
	ldmfd	sp!,{r0}
	msr	cpsr, r0
	ldmfd	sp!,{r0-r12, lr, pc}