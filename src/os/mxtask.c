#include <easyos.h>

struct task *current = NULL;

void mico_os_find_next(void)
{
	current = current->next;
}

void mico_os_set_current(struct task *taskobj)
{
	uint32_t flags = irq_lock_save();
	current = taskobj;
	irq_unlock_restore(flags);
}

struct task *mico_os_get_current(void)
{
	struct task *tsk = NULL;
	uint32_t flags = irq_lock_save();
	tsk = current;
	irq_unlock_restore(flags);
	return tsk;
}

int mico_os_task_init(struct task *taskobj, void (*entry_func)(void *), 
	void *args, void *stkbase, uint32_t stksz, const char *name)
{
	uint32_t *pstp = NULL;
	ulong stktop = (ulong)stkbase + stksz;

	stktop = ALIGN_DOWN(stktop, sizeof(ulong));
	stktop -= sizeof(ulong);
	memset(taskobj, 0, sizeof(*taskobj));
	memset(stkbase, 0, stksz);

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


