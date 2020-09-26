#include <easyos.h>

int main(void)
{
	//bsp_timer_app_init();

	uint32_t val = 123;

	uint32_t valdwn = ALIGN_DOWN(val, 4);
	uint32_t valup = ALIGN_SIZE(val, 4);

	printk("valdwn = 0x%x, valup = 0x%x \n", valdwn, valup);

	return 0;
}
