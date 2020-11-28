#include <easyplat.h>

/* 16KB align for pgd */
static __attribute__ ((aligned (0x4000)))
	unsigned int mmu_pgd_table[MMU_PGD_ITEMS];
/* 1KB align for PTE */
static __attribute__ ((aligned (0x400)))
	unsigned int mmu_pte_table[MMU_PGD_ITEMS][MMU_PTE_ITEMS];

/* now we using flatmap virtualaddr as index
 * map to same physical addr
 * we can also map virtaladdr to differnt physical addr
*/
int arch_mmu_page_table_create(void)
{
	unsigned int i = 0,j = 0;
	unsigned int pentry_pte = 0;
	unsigned int *pgd_table = NULL;

	/* AP=01010101 4KB-MAP */
	pentry_pte = MMU_PAGE_MAP_ENTRY_ATTR;
	pgd_table = (unsigned int *)mmu_pgd_table;

	for (i = 0; i < MMU_PGD_ITEMS; i++) {
		pgd_table[i] = ((unsigned int)mmu_pte_table[i]) | MMU_PAGE_MAP;
		for (j = 0; j < MMU_PTE_ITEMS; j++) {
			mmu_pte_table[i][j] = pentry_pte;
			pentry_pte += PAGE_SIZE;
		}
	}

	return 0;
}

int arch_mmu_section_table_create(void)
{
	unsigned int i = 0;
	unsigned int pentry = 0;
	unsigned int *ptable = NULL;

	pentry = MMU_SECTION_MAP;
	ptable = (unsigned int *)mmu_pgd_table;
	for (i = 0; i < MMU_PGD_ITEMS; i ++){
		ptable[i] = pentry;
		pentry += SECTION_SIZE;
	}

	return 0;
}

int arch_mmu_init(void)
{
#if 0
	arch_mmu_section_table_create();
#else
	arch_mmu_page_table_create();
#endif
	arch_mmu_set_ttb(mmu_pgd_table);
	arch_mmu_flush_all_tlb();
	arch_mmu_domain_init();
	arch_mmu_enable();

	return 0;
}

