#include <easyos.h>

static struct sema sigevent;

void app_init(void)
{
	sem_init(&sigevent, 0);
}

void demox_thread_func(void *args)
{
	(void)args;

	bsp_apptimer_init();
	while (1) {
		printk("runing ... demox_thread_func \n");
		sem_pend(&sigevent);
	}
}

void demoy_thread_func(void *args)
{
	uint32_t data = (uint32_t)args;

	while (1) {
		sem_pend(&sigevent);
		data++;
	}
}

void demoz_thread_func(void *args)
{
	uint32_t data = (uint32_t)args;

	while (1) {
		data++;
		printk("runing ... demoz_thread_func \n");
		sem_post(&sigevent);
	}
}

void demoz_thread_idle(void *args)
{
	(void)args;

	while (1) {
		asm volatile ("nop");
		printk("runing ... demoz_thread_idle \n");
	}
}