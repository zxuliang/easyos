#include <easyos.h>

extern void mxos_start(void);




char *strncpy(char *dest, const char *src, size_t n)
{
	size_t i;

	for (i = 0; i < n && src[i] != '\0'; i++)
	   dest[i] = src[i];
	for ( ; i < n; i++)
	   dest[i] = '\0';

	return dest;
}

void memset(void *base, char type, uint32_t size)
{
	uint32_t i = 0;
	for (i = 0; i < size; i++) {
		((char *)base)[i] = type;
	}
}

int mx_task_init(struct task *taskobj, void (*entry_func)(void *), 
	void *args, void *stkbase, uint32_t stksz, const char *name)
{
	uint32_t *pstp = NULL;
	ulong stktop = (ulong)stkbase + stksz;

	stktop = ALIGN_DOWN(stktop, sizeof(ulong));
	stktop -= sizeof(ulong);
	memset(taskobj, 0, sizeof(*taskobj));
	memset(stkbase, 0, stksz);

	printk("stkbase = 0x%x \n", stkbase);
	printk("stktop_x = 0x%x \n", (ulong)stkbase + stksz);
	printk("stktop_y = 0x%x \n", stktop);

	strncpy(taskobj->name, name, TASK_NAME_SIZE);
	taskobj->stack_base = stkbase;
	taskobj->stack_size = stktop - (ulong)stkbase;
	taskobj->entry = entry_func;
	taskobj->args = args;

	pstp = (uint32_t *)stktop;
	*pstp-- = (uint32_t)entry_func;		/* pc */
	*pstp-- = (uint32_t)entry_func; 	/* lr */
	*pstp-- = 0x12121212;			/* r12 */
	*pstp-- = 0x11111111;			/* r11 */
	*pstp-- = 0x10101010;			/* r10 */
	*pstp-- = 0x99999999;			/* r09 */
	*pstp-- = 0x88888888;			/* r08 */
	*pstp-- = 0x77777777;			/* r07 */
	*pstp-- = 0x66666666;			/* r06 */
	*pstp-- = 0x55555555;			/* r05 */
	*pstp-- = 0x44444444;			/* r04 */
	*pstp-- = 0x33333333;			/* r03 */
	*pstp-- = 0x22222222;			/* r02 */
	*pstp-- = 0x11111111;			/* r01 */
	*pstp-- = (uint32_t)args;		/* r00 */
	*pstp-- = SVC_MODE;			/* cpsr: svc mode with interrupt enable */
	*pstp	= SVC_MODE;			/* spsr: svc mode with interrupt enable */

	taskobj->tsp = pstp;			/* save full down stack pointer */

	return 0;
}




struct task demo_thread, hello_thread;
static uint32_t demo_stack[128];
static uint32_t hello_stack[128];
struct task *current = &demo_thread;

void mxos_switch(void);

void mxos_find_next(void)
{
	current=current->next;
}

void demo_thread_func(void *args)
{
	uint32_t data = (uint32_t)args;

	while (1) {
		printk("This is %s with data 0x%x \n", current->name, data);
		mxos_switch();
	}
}

void demo_thread_app(void)
{
	mx_task_init(&demo_thread, demo_thread_func, (void *)0x1234, 
		demo_stack, 128 * sizeof(uint32_t), "demo");

	mx_task_init(&hello_thread, demo_thread_func, (void *)0x5678, 
		hello_stack, 128 * sizeof(uint32_t), "hello");

	demo_thread.next = &hello_thread;
	hello_thread.next = &demo_thread;

	mxos_start();
}
