#include <easyos.h>

void main_entry(void)
{
	int x = 1024;
	printk("Hello this is demo code 0x%x \n", x);
}

void dummy_show(void)
{
	printk("this is just dummy show printk \n");
}
