
/* void clean_dcache(void) */
.global clean_dcache
clean_dcache:
dloop:
	mrc	p15, 0, r15, c7, c10, 3
	bne	dloop
	bx	lr

/* void clean_invalid_dcache(void) */
.global clean_invalid_dcache
clean_invalid_dcache:
diloop:
	mrc	p15, 0, r15, c7, c14, 3
	bne	diloop
	bx	lr

/* void invalid_cache(void) */
.global invalid_cache
invalid_cache:
	mov	r0, #0
	mcr	p15, 0, r0, c7, c7, 0
	bx	lr

/* dcache take effect only <= arch_mmu is enable */
.global arch_dcache_enable
arch_dcache_enable:
	mrc	p15, 0, r0, c1, c0, 0
	orr	r0, r0, #4
	mcr	p15, 0, r0, c1, c0, 0
	bx	lr

.global arch_dcache_disable
arch_dcache_disable:
	mrc	p15, 0, r0, c1, c0, 0
	bic	r0, r0, #4
	mcr	p15, 0, r0, c1, c0, 0
	bx	lr

.global arch_icache_enable
arch_icache_enable:
	mrc	p15, 0, r0, c1, c0, 0
	orr	r0, r0, #0x1000
	mcr	p15, 0, r0, c1, c0, 0
	bx	lr

.global arch_icache_disable
arch_icache_disable:
	mrc	p15, 0, r0, c1, c0, 0
	bic	r0, r0, #0x1000
	mcr	p15, 0, r0, c1, c0, 0
	bx	lr

