#include <easyos.h>

static u32 task_rdy_qmask = 0;
struct list_head task_rdy_queue[TASK_MAX_PRI];

struct task *current = NULL;
static volatile u32 yield_flag = 0;

void mico_os_init(void)
{
	int i = 0;

	task_rdy_qmask = 0;
	for (i = 0; i < TASK_MAX_PRI; i++) {
		INIT_LIST_HEAD(&task_rdy_queue[i]);
	}
}

void task_rdyq_set_mask(uint32_t pri)
{
	setbitsl(&task_rdy_qmask, BIT_MASK(pri));
}

void task_rdyq_clr_mask(uint32_t pri)
{
	clrbitsl(&task_rdy_qmask, BIT_MASK(pri));
}

void task_rdyq_test_clr(uint32_t pri)
{
	if (list_empty(&task_rdy_queue[pri])) {
		clrbitsl(&task_rdy_qmask, BIT_MASK(pri));
	}
}

int mico_os_task_init(struct task *taskobj, void (*entry_func)(void *),
	void *args, uint32_t flags, void *stkbase, uint32_t stksz,
	const char *name, int prior)
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
	taskobj->priority = prior;
	taskobj->status = TASK_STATE_READY;
	INIT_LIST_HEAD(&taskobj->tsknode);
	list_add_tail(&taskobj->tsknode, &task_rdy_queue[taskobj->priority]);
	setbitsl(&task_rdy_qmask, BIT_MASK(taskobj->priority));

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
	uint32_t flags;
	struct task *tsk = NULL;

	flags = irq_lock_save();
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
	uint32_t flags = 0;

	flags = irq_lock_save();
	if (intrpt_nest_counter == 0 && schedule_lock_counter == 0) {
		mico_os_ctx_switch();
	} else {
		mico_os_intrpt_switch();
	}
	irq_unlock_restore(flags);
}

void mico_os_schedule_lock(void)
{
	uint32_t flags = irq_lock_save();
	schedule_lock_counter++;
	irq_unlock_restore(flags);
}

void mico_os_schedule_unlock(void)
{
	uint32_t flags = irq_lock_save();
	if (schedule_lock_counter > 0) {
		schedule_lock_counter--;
	}
	irq_unlock_restore(flags);
}

void mico_os_yield(void)
{
	u32 flag = irq_lock_save();
	yield_flag = 1;
	irq_unlock_restore(flag);
	mico_os_schedule();
}

void mico_os_find_next(void)
{
	uint32_t i = 0;

	if (yield_flag) {
		yield_flag = 0;
		list_move_tail(&current->tsknode, &task_rdy_queue[i]);
		current = list_first_entry(&task_rdy_queue[i], struct task, tsknode);
		return;
	}

	i = __builtin_ctz(task_rdy_qmask);
	current = list_first_entry(&task_rdy_queue[i], struct task, tsknode);
}
