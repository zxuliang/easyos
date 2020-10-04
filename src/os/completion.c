#include <easyos.h>

void init_completion(struct completion *eventobj)
{
	INIT_LIST_HEAD(&eventobj->evtwq);
	eventobj->done = 0;
}

void wait_for_completion(struct completion *eventobj)
{
	uint32_t flags;
	struct task *curt = NULL;

	curt = mico_os_get_current();
	flags = irq_lock_save();
	if (0 == eventobj->done) {
		list_move_tail(&curt->tsknode, &eventobj->evtwq);
		mico_os_ctx_switch();
	}
	eventobj->done--;
	irq_unlock_restore(flags);
}

void complete(struct completion *eventobj)
{
	uint32_t flags;
	flags = irq_lock_save();
	eventobj->done++;

	if (list_empty(&eventobj->evtwq)) {
		/* nothing */
	} else {
		list_move_tail(eventobj->evtwq.next, &task_rdy_queue);
	}
	irq_unlock_restore(flags);
}

void complete_all(struct completion *eventobj)
{
	uint32_t flags;
	struct list_head *pos = NULL;
	struct list_head *n = 0;
	
	flags = irq_lock_save();
	eventobj->done = 0xFFFF0000;
	list_for_each_safe(pos, n, &eventobj->evtwq) {
		list_move_tail(pos, &task_rdy_queue);
	}
	irq_unlock_restore(flags);
}
