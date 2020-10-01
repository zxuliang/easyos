#include <easyos.h>

struct timer_st
{
	uint32_t index;
	uint32_t base;
};

static struct timer_st sys_tm[4] = {
	[0] = {
		.index = 0,
		.base = TIMER0_BASE,
	},

	[1] = {
		.index = 1,
		.base = TIMER1_BASE,
	},

	[2] = {
		.index = 2,
		.base = TIMER2_BASE,
	},

	[3] = {
		.index = 3,
		.base = TIMER3_BASE,
	},
};

void bsp_timer_init(uint32_t index)
{
	writel((sys_tm[index].base + TLOAD), 0x0);
	writel((sys_tm[index].base + TVALUE), 0xFFFFFFFF);
	writel((sys_tm[index].base + TRIS), 0);
	writel((sys_tm[index].base + TMIS), 0);
	writel((sys_tm[index].base + TLOAD), 0x4000);			/* load value */
	writel((sys_tm[index].base + TCNTL), TIMER_DEBUG_VAL);
	writel((sys_tm[index].base + TBGLOAD), 0x4000);			/* autoload value */
}

void bsp_timer_start(uint32_t index)
{
	setbitsl(sys_tm[index].base + TCNTL, ((1 << TIMER_EN_BIT)));
}

void bsp_timer_stop(uint32_t index)
{
	clrbitsl(sys_tm[index].base + TCNTL, (1 << TIMER_EN_BIT));
}

void timer_irq_handler(void *data)
{
	struct timer_st *ptmr = data;
	writel((sys_tm[ptmr->index].base + TINTCLR), 0xFFFFFFFF);
	printk("timer%u.... fired \n", ptmr->index);
}

void bsp_timer_app_init(void)
{
	bsp_timer_init(TIMER0);
	request_irq(TIM01_BIT_IRQ, 0, timer_irq_handler, &sys_tm[TIMER0], "timer0");
	bsp_timer_start(TIMER0);
}