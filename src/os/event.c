#include <easyos.h>

void init_event(struct event *eventobj, uint32_t mask)
{
	INIT_LIST_HEAD(&eventobj->evtwq);
	eventobj->data = 0;
	eventobj->mask = mask;
}

void wait_event(struct event *eventobj)
{
	uint32_t flags = 0;
	uint32_t datax;
	struct task *curtask = NULL;

	flags = irq_lock_save();
	curtask = mico_os_get_current();
	while (1) {
		datax = eventobj->data & eventobj->mask;
		if (datax != eventobj->mask) {
			curtask->status = TASK_STATE_BLOCK;
			list_move_tail(&curtask->tsknode, &eventobj->evtwq);
			irq_unlock_restore(flags);
			mico_os_schedule();
			flags = irq_lock_save();
		} else {
			eventobj->data = 0;
			curtask->status = TASK_STATE_READY;
			break;
		}
	}
	
	irq_unlock_restore(flags);
}

void wakeup_event(struct event *eventobj, uint32_t dmask)
{
	uint32_t flags;
	uint32_t datax;
	struct list_head *pos, *n;

	flags = irq_lock_save();
	eventobj->data |= dmask;
	datax = eventobj->data & eventobj->mask;
	if (datax == eventobj->mask) {
		if (list_empty(&eventobj->evtwq)) {
		} else {
			list_for_each_safe(pos, n, &eventobj->evtwq) {
				list_move_tail(pos, &task_rdy_queue);
			}
		}
	}
	irq_unlock_restore(flags);
}
