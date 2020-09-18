#include <easyos.h>

.global demo_entry

.text
.align  2
.global sys_start
.arch armv5tej
.syntax unified
.arm
.fpu softvfp
.type   sys_start, %function

sys_start:
	ldr sp, =__svc_stack_top__
	
	bl	main_entry
	b .
