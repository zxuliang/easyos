#include <easyos.h>

struct task demox_thread;
struct task demoy_thread;
struct task demoz_thread;
struct task idle_thread;

static uint32_t demox_stack[128];
static uint32_t demoy_stack[128];
static uint32_t demoz_stack[128];
static uint32_t idle_stack[128];

void app_init(void);
void demox_thread_func(void *args);
void demoy_thread_func(void *args);
void demoz_thread_func(void *args);
void demoz_thread_idle(void *args);

void app_main(void)
{
	memset(&demox_thread, 0, sizeof(demox_thread));
	memset(&demoy_thread, 0, sizeof(demoy_thread));
	memset(&demoz_thread, 0, sizeof(demoz_thread));

	mico_os_task_init(&demox_thread, demox_thread_func, (void *)0x11110000,0,
		demox_stack, 128 * sizeof(uint32_t), "demox", 1);

	mico_os_task_init(&demoy_thread, demoy_thread_func, (void *)0x22220000,0,
		demoy_stack, 128 * sizeof(uint32_t), "demoy", 2);

	mico_os_task_init(&demoz_thread, demoz_thread_func, (void *)0x33330000,0,
		demoz_stack, 128 * sizeof(uint32_t), "demoz", 3);


	mico_os_task_init(&idle_thread, demoz_thread_idle, (void *)0x00000000,0,
		idle_stack, 128 * sizeof(uint32_t), "idle", 31);

	mico_os_start();
}

int main(void)
{
	app_init();
	app_main();

	return 0;
}
