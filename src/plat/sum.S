#include <easyos.h>

/* int sum(int x, int y, int z, int m, int n) */

.text
.align 2
.global sum
.type sum,%function

sum:
	stmfd	sp!, {fp, lr}
	add	fp , sp, #4
	
	add	r0, r0, r1
	add	r0, r0, r2
	add	r0, r0, r3
	ldr	r1, [fp, #4]
	add	r0, r0, r1

	sub	sp, fp, #4
	ldmfd	sp!, {fp, lr}
	bx	lr
	

