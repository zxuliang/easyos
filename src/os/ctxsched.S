
.text 
.align 2
.extern current
.global mxos_start
/* void mxos_start(void) */
mxos_start:
	ldr	r0,	=current
	ldr	r0,	[r0]
	ldr	sp, 	[r0, #0]		/* task.tsp */
	ldmfd	sp!,	{r0}
	msr	spsr,	r0			/* spsr */
	ldmfd	sp!,	{r0}
	msr	cpsr,	r0			/* cpsr */
	ldmfd	sp!,	{r0-r12, lr, pc}	/* r0-r12, lr, pc */