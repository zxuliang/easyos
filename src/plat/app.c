#include <easyos.h>

struct task demo;
struct task hello;

static uint32_t hello_stack[128];
static uint32_t demo_stack[128];

void thread_func(void *args)
{
	uint32_t flags = 0;
	uint32_t data = (uint32_t)args;

	(void)flags;

	while (1) {
		data++;
		printk(" %s with 0x%x \n", current->name, data);

#ifndef MICO_OS_INTRPT_CTX_SWITCH
		flags = irq_lock_save();
		mico_os_ctx_switch();
		irq_unlock_restore(flags);	
#else
	
#endif

		
	}
}

void app_main(void)
{
	mico_os_task_init(&demo, thread_func, (void *)0x1234,
		demo_stack, 128 * sizeof(uint32_t), "demo");

	mico_os_task_init(&hello, thread_func, (void *)0x5678,
		hello_stack, 128 * sizeof(uint32_t), "hello");

	demo.next = &hello;
	hello.next =&demo;

	bsp_apptimer_init();

	mico_os_start();
}

int main(void)
{
	app_main();
	return 0;
}
