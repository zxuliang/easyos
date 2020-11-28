
#ifndef __EASY_PLAT_HEADER_H__
#define __EASY_PLAT_HEADER_H__

#define TASK_MAX_PRI	(32)
#define TASK_NAME_SIZE	(15)

#define TASK_STATE_BLOCK (0xFF)
#define TASK_STATE_READY (0xEE)
#define TASK_STATE_RUNNING (0xAA)

/*---------------------------------------------------------*/
#define NO_IRQ_MASK		(0x80)
#define NO_FIQ_MASK		(0x40)
#define NO_INTRPT_MASK		(0xC0)

#define USR_MODE		(0x10)
#define FIQ_MODE		(0x11)
#define IRQ_MODE		(0x12)
#define SVC_MODE		(0x13)
#define ABT_MODE		(0x1B)
#define SYS_MODE		(0x1F)

/*---------------------------------------------------------*/
#define NR_IRQ			(32)
#define VIC_BASE		(0x10140000)
#define VIC_IRQ_STATUS		(0x00)
#define VIC_FIQ_STATUS		(0x04)
#define VIC_RAW_INTR_STATUS	(0x08)
#define VIC_INT_SEL		(0x0c)
#define VIC_INT_ENABLE		(0x10)	/* rw */
#define VIC_INT_ENABLE_CLR	(0x14)	/* w */
#define VIC_VECT_ADDR_EOI	(0x30)
#define VIC_VECT_ADDR0		(0x100)
#define VIC_VECT_ADDR(n)	(VIC_VECT_ADDR0 + n * 4)
#define VIC_VECT_CTRL0		(0x200)
#define VIC_VECT_CTRL(n)	(VIC_VECT_CTRL0 + n * 4)

/*---------------------------------------------------------*/
/* also as irqslot */
#define WDT_BIT_IRQ		(0)
#define SOFT_BIT_IRQ		(1)
#define COM_RX_BIT_IRQ		(2)		/* MODEM */
#define COM_TX_BIT_IRQ		(3)
#define TIM01_BIT_IRQ		(4)
#define TIM23_BIT_IRQ		(5)

/*---------------------------------------------------------*/
#define TIMER0_BASE		(0x101E2000)
#define TIMER1_BASE		(0x101E2020)
#define TIMER2_BASE		(0x101E3000)
#define TIMER3_BASE		(0x101E3020)

#define TIMER0		(0)
#define TIEMR1		(1)
#define TIMER2		(2)
#define TIMER3		(3)

#define TIMER_EN_BIT		(7)		/* 1: enable, 0: disable */
#define TIMER_MODE_BIT		(6)		/* 0: freerun; 1: priod */
#define TIMER_INT_EN_BIT 	(5)		/* 1: enable,  0: disable*/
#define TIMER_PRE_DIVDER	(2)		/* divider:00=1,01=8,10=256 01 for divide by 8 */
#define TIMER_IS_32_BIT		(1)		/* 1: 32, 0: 16 */
#define TIMER_TYPE_BIT		(0)		/* 1: wraparound, 0: oneshot */

#define TLOAD 			(0x00) 		/* load, change the timer right now */
#define TVALUE 			(0x04) 		/* value of current */
#define TCNTL 			(0x08) 		/* control */
#define TINTCLR 		(0x0C) 		/* interrupt clear */
#define TRIS 			(0x10) 		/* raw interrupt status */
#define TMIS 			(0x14)		/* mask interrupt status */
#define TBGLOAD 		(0x18) 		/* background locad,  change next reach zero */

#define TIMER_DEBUG_VAL		(0x66)		/* 32-bit priod, wraparound, interrupt-enable , div/8 */

/*---------------------------------------------------------*/
#define TXFE			(0x80)
#define RXFF			(0x40)
#define TXFF			(0x20)
#define RXFE			(0x10)
#define BUSY			(0x08)
#define UDR			(0x00)
#define UFR			(0x18)
#define UART0_BASE 		(0x101F1000)
#define UART1_BASE 		(0x101F2000)
#define UART2_BASE 		(0x101F3000)
#define UART3_BASE 		(0x10009000)
/*---------------------------------------------------------*/
#define BIT_MASK(x)		(1 << (x))
/*---------------------------------------------------------*/

#ifndef __ASM__

#include <libutils.h>
#include <os.h>

extern void printk(const char *fmt, ...);
/*--------------------------------------------------------*/
extern void arch_wfi_idle(void);
/*---------------------------------------------------------*/
extern void irq_enable(void);
extern void irq_disable(void);
extern uint32_t irq_lock_save(void);
extern void irq_unlock_restore(uint32_t flags);

/*---------------------------------------------------------*/
extern void timer_irq_handler(void *data);
extern void bsp_apptimer_init(void);
extern void bsp_apptimer_stop(void);

/*---------------------------------------------------------*/
extern uint32_t system_systick;
extern uint32_t intrpt_context_switch;
/*--------------------------------------------------------*/
/* AP=01, seciont-map nocache-nowribuf*/
#define MMU_SECTION_MAP		(0x412)

/* finepage-table 4KB */
#define MMU_PAGE_MAP			(0x11)
#define MMU_PAGE_MAP_ENTRY_ATTR		(0x552)

#define MMU_BIT_CACHE	(1 << 3)
#define MMU_BIT_WTBUF	(1 << 2)

#define MMU_PGD_ITEMS	(4096)
#define MMU_PTE_ITEMS	(256)

#define SECTION_SIZE	(0x100000)	/* 1MB SIZE */
#define PAGE_SIZE	(0x1000)	/* 4KB SIZE */

extern void arch_mmu_table_create(void);
extern void arch_mmu_flush_all_tlb(void);
extern void arch_mmu_set_ttb(void *ttb);
extern void arch_mmu_domain_init(void);
extern void arch_mmu_enable(void);

extern void arch_dcache_enable(void); /* only mmu enable, dcache can be work*/
extern void arch_dcache_disable(void);
extern void arch_icache_enable(void);
extern void arch_icache_disable(void);
/*---------------------------------------------------------*/

/*---------------------------------------------------------*/
typedef void (*irq_handler_t)(void *);
struct irq_manager_st {
	uint32_t irq;
	uint32_t type;
	void *data;
	irq_handler_t handler;
	const char *name;
};
extern void request_irq(uint32_t irq, uint32_t type,
	irq_handler_t handler, void *data, const char *name);

extern uint32_t intrpt_nest_counter;
extern uint32_t system_systick;
extern uint32_t intrpt_context_switch;
extern uint32_t schedule_lock_counter;
/*-------------------------------------------------------*/

#endif

#endif
