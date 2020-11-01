#include <easyos.h>

void sem_init(struct sema *sema, int cnt)
{
	int i = 0;

	sema->evtwq_mask = 0;
	sema->value = cnt;
	sema->init_value = cnt;
	for (i = 0; i < TASK_MAX_PRI; i++) {
		INIT_LIST_HEAD(&sema->evtwq[i]);
	}
}

void sem_pend(struct sema *sema)
{
	uint32_t prio = 0;
	uint32_t flags = 0;
	struct task *ptask = NULL;

	flags = irq_lock_save();

	if (--sema->value < 0) {
		ptask = mico_os_get_current();
		prio = ptask->priority;

		list_move_tail(&ptask->tsknode, &sema->evtwq[prio]);
		task_rdyq_test_clr(ptask->priority);
		ptask->status = TASK_STATE_BLOCK;
		setbitsl(&sema->evtwq_mask, BIT_MASK(ptask->priority));

		irq_unlock_restore(flags);
		mico_os_schedule();
	} else {
		irq_unlock_restore(flags);
	}
}

void sem_post(struct sema *sema)
{
	uint32_t i = 0;
	uint32_t flags = 0;
	struct task *ptask = NULL;

	flags = irq_lock_save();

	sema->value++;
	if (sema->value > sema->init_value) {
		sema->value = sema->init_value;
	}

	/* some one wait on this semaphore */
	if (sema->evtwq_mask) {

		i = __builtin_ctz(sema->evtwq_mask);
		ptask = list_first_entry(&sema->evtwq[i], struct task, tsknode);
		ptask->status = TASK_STATE_READY;
		task_rdyq_set_mask(ptask->priority);
		list_move_tail(&ptask->tsknode, &task_rdy_queue[ptask->priority]);

		if (list_empty(&sema->evtwq[i])) {
			clrbitsl(sema->evtwq_mask, BIT_MASK(ptask->priority));
		}

		if (current->priority > ptask->priority) {
			irq_unlock_restore(flags);
			mico_os_schedule();
		} else {
			irq_unlock_restore(flags);
			mico_os_yield();
		}

		return;
	}

	irq_unlock_restore(flags);
}
