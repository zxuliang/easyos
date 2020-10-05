#include <easyos.h>

struct task *current = NULL;
struct task *nextrdy = NULL;
struct task *gtask = NULL;


LIST_HEAD(task_rdy_queue);
LIST_HEAD(task_wait_queue);

uint32_t schedule_lock_counter = 0;

int mico_os_task_init(struct task *taskobj, void (*entry_func)(void *), 
	void *args, uint32_t flags, void *stkbase, uint32_t stksz, const char *name)
{
	uint32_t *pstp = NULL;
	ulong stktop = (ulong)stkbase + stksz;

	stktop = ALIGN_DOWN(stktop, sizeof(ulong));
	memset(taskobj, 0, sizeof(*taskobj));
	memset(stkbase, 0, stksz);

	strncpy(taskobj->name, name, TASK_NAME_SIZE);
	taskobj->stack_base = stkbase;
	taskobj->stack_size = stktop - (ulong)stkbase;
	taskobj->entry = entry_func;
	taskobj->args = args;
	taskobj->next = current;
	current = taskobj;
	gtask = current;
	INIT_LIST_HEAD(&taskobj->tsknode);
	list_add_tail(&taskobj->tsknode, &task_rdy_queue);
	
	/* filled with full-down-stack-style, stmfd sp!,{xxxx} */
	pstp = (uint32_t *)stktop;
	*(--pstp) = (uint32_t)entry_func;	/* pc */
	*(--pstp) = (uint32_t)entry_func; 	/* lr */
	*(--pstp) = 0x12121212;			/* r12 */
	*(--pstp) = 0x11111111;			/* r11 */
	*(--pstp) = 0x10101010;			/* r10 */
	*(--pstp) = 0x99999999;			/* r09 */
	*(--pstp) = 0x88888888;			/* r08 */
	*(--pstp) = 0x77777777;			/* r07 */
	*(--pstp) = 0x66666666;			/* r06 */
	*(--pstp) = 0x55555555;			/* r05 */
	*(--pstp) = 0x44444444;			/* r04 */
	*(--pstp) = 0x33333333;			/* r03 */
	*(--pstp) = 0x22222222;			/* r02 */
	*(--pstp) = 0x11111111;			/* r01 */
	*(--pstp) = (uint32_t)args;		/* r00 */
	*(--pstp) = SVC_MODE;			/* cpsr: svc mode with interrupt enable */
	*(--pstp) = SVC_MODE;			/* spsr: svc mode with interrupt enable */

	taskobj->tsp = pstp;			/* save full down stack pointer */

	return 0;
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

void mico_os_intrpt_switch(void)
{
	uint32_t flags = irq_lock_save();
	intrpt_context_switch = 1;
	irq_unlock_restore(flags);
}

void mico_os_schedule(void)
{
	uint32_t flags = irq_lock_save();
	mico_os_ctx_switch();
	irq_unlock_restore(flags);
}

void mico_os_schedule_lock(void)
{
	uint32_t flags = irq_lock_save();
	schedule_lock_counter++;
	irq_unlock_restore(flags);
}

/* schedule by fifo or priority if need */
void mico_os_find_next(void)
{
	list_move_tail(&current->tsknode, &task_rdy_queue);
	current = list_first_entry(&task_rdy_queue, struct task, tsknode);
}
