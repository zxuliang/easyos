#include <easyos.h>

uint32_t intrpt_context_switch = 0;
uint32_t intrpt_nest_counter = 0;
uint32_t schedule_lock_counter = 0;

static struct irq_manager_st irq_manager[NR_IRQ + 1];

void vic_irq_disable(uint32_t irq)
{
	setbitsl((VIC_BASE + VIC_INT_ENABLE_CLR), (1 << irq));
}

void vic_irq_enable(uint32_t irq)
{
	setbitsl((VIC_BASE + VIC_INT_ENABLE), (1 << irq));
}

void request_irq(uint32_t irq, uint32_t type, irq_handler_t handler, void *data, const char *name)
{
	vic_irq_disable(irq);
	irq_manager[irq].irq = irq;
	irq_manager[irq].handler = handler;
	irq_manager[irq].data = data;
	irq_manager[irq].name = name;
	vic_irq_enable(irq);
}

void do_irq_handle_ops(void)
{
	unsigned int i = 0;
	unsigned int vic_status = readl(VIC_BASE + VIC_IRQ_STATUS);

	intrpt_nest_counter++;
	for (i = 0; i < NR_IRQ; i++) {
		if (vic_status & (1 << i)) {
			if (irq_manager[i].handler) {
				irq_manager[i].handler(irq_manager[i].data);
			}
			writel((VIC_BASE + VIC_VECT_ADDR_EOI), 0);
		}
	}

	intrpt_nest_counter--;
	if (intrpt_nest_counter == 0 && schedule_lock_counter == 0) {
		intrpt_context_switch = 1;
	}
}