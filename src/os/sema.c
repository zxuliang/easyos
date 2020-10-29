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
	ptask = mico_os_get_current();

	prio = ptask->priority;
	if (--sema->value < 0) {
		list_move_tail(&ptask->tsknode, &sema->evtwq[prio]);
		ptask->status = TASK_STATE_BLOCK;
		sema->evtwq_mask |= (1 << ptask->priority);
		irq_unlock_restore(flags);
		mico_os_schedule();	/* giveup cpu to let other task run */
	} else {
		irq_unlock_restore(flags);
	}
}

void sem_post(struct sema *sema)
{
	uint32_t i = 0;
	uint32_t mask = 0;
	uint32_t flags = 0;
	struct task *ptask = NULL;

	flags = irq_lock_save();

	sema->value++;
	if (sema->value > sema->init_value) {
		sema->value = sema->init_value;
	}

	/* some one wait on this semaphore */
	if (sema->value <= 0 || sema->evtwq_mask) {
		for (i = 0; i < TASK_MAX_PRI; i++) {
			mask = 1 << i;
			if (!(mask & sema->evtwq_mask)) {
				continue;
			}

			ptask = list_first_entry(&sema->evtwq[i], struct task, tsknode);
			ptask->status = TASK_STATE_READY;
			list_move_tail(&ptask->tsknode, &task_rdy_queue[ptask->priority]);
			if (list_empty(&sema->evtwq[i])) {
				sema->evtwq_mask &= ~mask;
			}

			/* giveup cpu to high pri task */
			if (current->priority > ptask->priority) {
				irq_unlock_restore(flags);
				mico_os_schedule();
				return;
			}
		}
	}

	irq_unlock_restore(flags);
}
