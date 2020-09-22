#include <easyos.h>

.extern main_entry
.extern dummy_show

.text
.align  2
.global sys_start
.arch armv5tej
.syntax unified
.arm
.fpu softvfp
.type   sys_start, %function

sys_start:
	ldr	sp, =__svc_stack_top__
	bl	main_entry
	bl	dummy_show
	b	.
