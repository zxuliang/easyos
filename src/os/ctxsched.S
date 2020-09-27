
.text 
.align 2
.extern current
.global mxos_start
/* void mxos_start(void) */
mxos_start:
	ldr	r0,	=current
	ldr	r0,	[r0]
	ldr	sp, 	[r0, #0]		/* tsp */
	ldmfd	sp!,	{r0}
	msr	spsr,	r0			/* spsr */
	ldmfd	sp!,	{r0}
	msr	cpsr,	r0			/* cpsr */
	ldmfd	sp!,	{r0-r12, lr, pc}	/* r0-r12, lr, pc */

.global mxos_switch
mxos_switch:
	stmfd	sp!, {lr}
	stmfd	sp!, {r0-r12, lr}
	mrs 	r0, cpsr
	stmfd	sp!, {r0}
	mrs	r0, spsr
	stmfd	sp!, {r0}
	ldr	r0, =current
	ldr	r0, [r0]
	str	sp, [r0]

	bl	mxos_find_next

	ldr	r0, =current
	ldr	r0, [r0]
	ldr	sp, [r0]
	ldmfd	sp!,{r0}
	msr	spsr, r0
	ldmfd	sp!,{r0}
	msr	cpsr, r0
	ldmfd	sp!,{r0-r12, lr}
	ldmfd	sp!,{pc}