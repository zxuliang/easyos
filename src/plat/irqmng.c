#include <easyos.h>

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

	printk("do_irq_handler \n");

	for (i = 0; i < NR_IRQ; i++) {
		if (vic_status & (1 << i)) {
			if (irq_manager[i].handler) {
				irq_manager[i].handler(irq_manager[i].data);
			}
			writel((VIC_BASE + VIC_VECT_ADDR_EOI), 0);
		}
	}
}