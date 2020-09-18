OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
SEARCH_DIR(.)

ENTRY(sys_start)

SECTIONS
{
	. = 0x10000;

	.text :
	{
		KEEP(*(.isr_vector))
		*(.text*)
	}

	.data :
	{
		. = ALIGN(4);
		__data_start__ = .;
		*(vtable)
		*(.data*)
		. = ALIGN(4);
		__data_end__ = .;
	}

	.bss (NOLOAD):
	{
		. = ALIGN(4);
		__bss_start__ = .;
		*(.bss*)
		*(COMMON)
		. = ALIGN(4);
		__bss_end__ = .;
	}

	. = ALIGN(8);

	. = . + 0x1000;
	__svc_stack_top__ = .;
}
