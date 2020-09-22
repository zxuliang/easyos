#include <easyos.h>

extern int sum(int x, int y, int z, int m, int n);

void main_entry(void)
{
	int x = 1, y = 2, z = 3, m = 4, n = 5;
	int k = sum(x, y, z, m, n);
	printk("====This is main_entry=====k = %d \n", k);
}

void dummy_show(void)
{
	printk("this is just dummy show printk \n");
}


