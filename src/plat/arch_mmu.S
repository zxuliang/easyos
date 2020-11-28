#include <easyplat.h>

.global arch_mmu_table_create
arch_mmu_table_create:
	bx	lr

.global arch_mmu_flush_all_tlb
arch_mmu_flush_all_tlb:
	mov	r0, #0
	mcr	p15, 0, r0, c8, c7, 0
	bx	lr

.global arch_mmu_set_ttb
arch_mmu_set_ttb:
	mcr	p15, 0, r0, c2, c0, 0
	bx	lr

.global arch_mmu_domain_init
arch_mmu_domain_init:
	mov	r0, #1
	mcr	p15, 0, r0 , c3, c0, 0
	bx	lr

.global arch_mmu_enable
arch_mmu_enable:
	mrc	p15, 0, r0, c1, c0, 0
	orr	r0, r0, #1
	mcr 	p15, 0, r0, c1, c0, 0
	bx	lr
