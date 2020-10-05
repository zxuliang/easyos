#include <easyos.h>

struct task hello;
struct task world;
struct task third;

static uint32_t one_stack[128];
static uint32_t two_stack[128];
static uint32_t three_stack[128];

#define WAIT_BIT_MASK (0x3)

struct event sigevent;


void one_thread_func(void *args)
{
	uint32_t data = (uint32_t)args;
	while (1) {
		data++;
		printk(" TASK[%s] with 0x%x \n", current->name, data);
		wait_event(&sigevent);
	}
}

void two_thread_func(void *args)
{
	uint32_t data = (uint32_t)args;

	while (1) {
		data++;
	//	printk(" TASK[%s] with 0x%x \n", current->name, data);	
		wakeup_event(&sigevent, 0x1);
	}
}

void three_thread_func(void *args)
{
	uint32_t data = (uint32_t)args;

	while (1) {
		data++;
	//	printk(" TASK[%s] with 0x%x \n", current->name, data);
		wakeup_event(&sigevent, 0x2);
	}
}

void app_main(void)
{
	init_event(&sigevent, WAIT_BIT_MASK);

	mico_os_task_init(&world, one_thread_func, (void *)0x11110000,0,
		one_stack, 128 * sizeof(uint32_t), "one");

	mico_os_task_init(&hello, two_thread_func, (void *)0x22220000,0,
		two_stack, 128 * sizeof(uint32_t), "two");

	mico_os_task_init(&third, three_thread_func, (void *)0x33330000,0,
		three_stack, 128 * sizeof(uint32_t), "three");

	bsp_apptimer_init();

	mico_os_start();
}

int main(void)
{
	app_main();
	return 0;
}
