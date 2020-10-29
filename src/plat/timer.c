#include <easyos.h>

uint32_t system_systick = 0;

struct hwtimer
{
	uint32_t base;
};

struct hwtimer systmr = {
	.base = TIMER0_BASE,
};

void bsp_timer_init(void)
{
	writel((systmr.base + TLOAD), 0x0);
	writel((systmr.base + TVALUE), 0xFFFFFFFF);
	writel((systmr.base + TRIS), 0);
	writel((systmr.base + TMIS), 0);
	writel((systmr.base + TLOAD), 0x8000);		/* load value */
	writel((systmr.base + TCNTL), TIMER_DEBUG_VAL);
	writel((systmr.base + TBGLOAD), 0x8000);	/* autoload value */
}

void bsp_timer_start(void)
{
	setbitsl(systmr.base + TCNTL, ((1 << TIMER_EN_BIT)));
}

void bsp_timer_stop(void)
{
	clrbitsl(systmr.base + TCNTL, (1 << TIMER_EN_BIT));
}

void timer_irq_handler(void *data)
{
	struct hwtimer *ptmr = data;
	system_systick++;
	writel((ptmr->base + TINTCLR), 0xFFFFFFFF);
}

void bsp_apptimer_init(void)
{
	bsp_timer_init();
	request_irq(TIM01_BIT_IRQ, 0, timer_irq_handler, &systmr, "timer0");
	bsp_timer_start();
}

void bsp_apptimer_stop(void)
{
	bsp_timer_stop();
}