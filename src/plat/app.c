#include <easyos.h>

struct task demo_thread, hello_thread;
static uint32_t demo_stack[128];
static uint32_t hello_stack[128];


void demo_thread_func(void *args)
{
	uint32_t flags = 0;
	uint32_t data = (uint32_t)args;

	while (1) {
		printk("%s with 0x%x \n", mico_os_get_current()->name, data);
		flags = irq_lock_save();
		mico_os_ctx_switch();
		irq_unlock_restore(flags);
	}
}

void demo_thread_app(void)
{
	mico_os_task_init(&demo_thread, demo_thread_func, (void *)0x1234, 
		demo_stack, 128 * sizeof(uint32_t), "demo");

	mico_os_task_init(&hello_thread, demo_thread_func, (void *)0x5678, 
		hello_stack, 128 * sizeof(uint32_t), "hello");

	demo_thread.next = &hello_thread;
	hello_thread.next = &demo_thread;

	bsp_timer_app_init();

	mico_os_start();
}

int main(void)
{
	bsp_timer_app_init();
	return 0;
}
