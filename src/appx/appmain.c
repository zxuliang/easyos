#include <easyplat.h>

#define APP_HELLO_PRIO 10
#define APP_LINUX_PRIO 11

#define OS_CFG_TASK_STK_SIZE	(256)
#define  OS_CFG_TASK_STK_LIMIT      \
		((OS_CFG_TASK_STK_SIZE  * OS_CFG_TASK_STK_LIMIT_PCT_EMPTY) / 100u)

static OS_TCB helloTCB;
static OS_TCB linuxTCB;

static CPU_STK bufhello[OS_CFG_TASK_STK_SIZE];
static CPU_STK buflinux[OS_CFG_TASK_STK_SIZE];

static OS_SEM semtalk;

void task_funcx(void *data)
{
	OS_ERR err;
	char *name = data;

	while (1) {
		printk("This is task %s +++++\n", name);
		OSSemPend(&semtalk, 0, OS_OPT_PEND_BLOCKING, NULL, &err);
	}
}

void task_funcy(void *data)
{
	OS_ERR err;
	char *name = data;

	bsp_apptimer_init();
	while (1) {
		printk("This is task %s \n", name);
		OSSemPost(&semtalk, OS_OPT_POST_1, &err);
	}
}

void bsp_bss_clear(void)
{
	extern unsigned int __bss_start__[];
	extern unsigned int __bss_end__[];
	unsigned int *ptr = __bss_start__;

	while (ptr < __bss_end__) {
		*ptr = 0;
		ptr++;
	}
}

int main(void)
{
	OS_ERR err;

	OSInit(&err);

	OSSemCreate(&semtalk, "talk", 1, &err);

	OSTaskCreate(&helloTCB,
			"hello",
			task_funcx, (void *)"hello",
			APP_HELLO_PRIO,
			(CPU_STK *)bufhello,
			OS_CFG_TASK_STK_LIMIT,
			OS_CFG_TASK_STK_SIZE,
			0,
			2,
			NULL,
			(OS_OPT_TASK_STK_CHK | (OS_OPT)(OS_OPT_TASK_STK_CLR | OS_OPT_TASK_NO_TLS)),
			&err);

	OSTaskCreate(&linuxTCB,
		"linux",
		task_funcy,
		(void *)"linux",
		APP_LINUX_PRIO,
		(CPU_STK *)buflinux,
		OS_CFG_TASK_STK_LIMIT,
		OS_CFG_TASK_STK_SIZE,
		0,
		4,
		NULL,
		(OS_OPT_TASK_STK_CHK | (OS_OPT)(OS_OPT_TASK_STK_CLR | OS_OPT_TASK_NO_TLS)),
		&err);

	OSStart(&err);

	return 0;
}
