#include <easyos.h>

struct task hello;
struct task world;
struct task third;

static uint32_t one_stack[128];
static uint32_t two_stack[128];
static uint32_t three_stack[128];

struct completion eventcmpt;


void one_thread_func(void *args)
{
	uint32_t flags = 0;
	uint32_t data = (uint32_t)args;

	(void)flags;
	while (1) {
		wait_for_completion(&eventcmpt);
		wait_for_completion(&eventcmpt);
		data++;
		printk(" TASK[%s] with 0x%x \n", current->name, data);
	}
}

void two_thread_func(void *args)
{
	uint32_t flags = 0;
	uint32_t data = (uint32_t)args;

	(void)flags;

	while (1) {
		data++;
		printk(" TASK[%s] with 0x%x \n", current->name, data);	
		mico_os_intrpt_switch();
	}
}

void three_thread_func(void *args)
{
	uint32_t flags = 0;
	uint32_t data = (uint32_t)args;

	(void)flags;

	while (1) {
		data++;
		printk(" TASK[%s] with 0x%x \n", current->name, data);
		complete(&eventcmpt);
		mico_os_intrpt_switch();
	}
}

void app_main(void)
{
	init_completion(&eventcmpt);

	mico_os_task_init(&world, one_thread_func, (void *)0x1234,
		one_stack, 128 * sizeof(uint32_t), "one");

	mico_os_task_init(&hello, two_thread_func, (void *)0x5678,
		two_stack, 128 * sizeof(uint32_t), "two");

	mico_os_task_init(&third, three_thread_func, (void *)0x5678,
		three_stack, 128 * sizeof(uint32_t), "three");

	bsp_apptimer_init();

	mico_os_start();
}

int main(void)
{
	app_main();
	return 0;
}
