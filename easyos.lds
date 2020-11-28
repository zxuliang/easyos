OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
SEARCH_DIR(.)

ENTRY(reset_handler)
SECTIONS
{
	. = 0x10000;
	.text : { *(.text) }
	.data : { *(.data) }
	. = ALIGN(8);

	__bss_start__ = .;
	.bss : { *(.bss) }
	. = ALIGN(8);
	__bss_end__ = .;

	. = ALIGN(8);
	. = . + 0x1000; 			/* 4kB of SVC stack space */
	svc_stack_top = .;
	. = . + 0x1000; 			/* 4kB of IRQ stack space */
	irq_stack_top = .;
}
