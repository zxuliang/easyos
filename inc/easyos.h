
#ifndef __EASY_READ_WRITE_H__
#define __EASY_READ_WRITE_H__


#define SYSTEM_HAS_OS		(0)


/*
* context-relate-type-two: {R0-R12,LR,PC,CPSR,SPSR}
*/

#define NO_IRQ_MASK		(0x80)
#define NO_FIQ_MASK		(0x40)
#define NO_INTRPT_MASK 	(0xC0)

#define USR_MODE	(0x10)
#define FIQ_MODE	(0x11)
#define IRQ_MODE	(0x12)
#define SVC_MODE	(0x13)
#define ABT_MODE	(0x1B)
#define SYS_MODE	(0x1F)

/***************VIC******************/

#define NR_IRQ				(32)

#define VIC_BASE 			(0x10140000)

#define VIC_IRQ_STATUS		(0x00)
#define VIC_FIQ_STATUS		(0x04)
#define VIC_RAW_INTR_STATUS	(0x08)
#define VIC_INT_SEL			(0x0c)
#define VIC_INT_ENABLE		(0x10)	/* rw */
#define VIC_INT_ENABLE_CLR	(0x14)	/* w */
#define VIC_VECT_ADDR_EOI	(0x30)
#define VIC_VECT_ADDR0		(0x100)
#define VIC_VECT_ADDR(n)	(VIC_VECT_ADDR0 + n * 4)
#define VIC_VECT_CTRL0		(0x200)
#define VIC_VECT_CTRL(n)	(VIC_VECT_CTRL0 + n * 4)

#define WDT_BIT_IRQ			(0)
#define SOFT_BIT_IRQ		(1)
#define COM_RX_BIT_IRQ		(2)		/* MODEM */
#define COM_TX_BIT_IRQ		(3)
#define TIM01_BIT_IRQ		(4)
#define TIM23_BIT_IRQ		(5)

/**************Counter-Timer*********/
#define TIMER0_BASE		(0x101E2000)
#define TIMER1_BASE		(0x101E2020)
#define TIMER2_BASE		(0x101E3000)
#define TIMER3_BASE		(0x101E3020)

#define TIMER0 (0)
#define TIEMR1 (1)
#define TIMER2 (2)
#define TIMER3 (3)

#define TIMER_EN_BIT		(7)		/* 1: enable, 0: disable */
#define TIMER_MODE_BIT		(6)		/* 0: freerun; 1: priod */
#define TIMER_INT_EN_BIT 	(5)		/* 1: enable,  0: disable*/
#define TIMER_PRE_DIVDER	(2)		/* divider:00=1,01=8,10=256 01 for divide by 8 */
#define TIMER_IS_32_BIT		(1)		/* 1: 32, 0: 16 */
#define TIMER_TYPE_BIT		(0)		/* 1: wraparound, 0: oneshot */

#define TLOAD 				0x00 	/* load, change the timer right now */
#define TVALUE 				0x04 	/* value of current */
#define TCNTL 				0x08 	/* control */
#define TINTCLR 			0x0C 	/* interrupt clear */
#define TRIS 				0x10 	/* raw interrupt status */
#define TMIS 				0x14	/* mask interrupt status */
#define TBGLOAD 			0x18 	/* background locad,  change next reach zero */

#define TIMER_DEBUG_VAL		(0x66)	/* 32-bit priod, wraparound, interrupt-enable , div/8 */

/************************UART******************/
#define TXFE 0x80
#define RXFF 0x40
#define TXFF 0x20
#define RXFE 0x10
#define BUSY 0x08
#define UDR 0x00
#define UFR 0x18
#define UART0_BASE 		(0x101F1000)
#define UART1_BASE 		(0x101F2000)
#define UART2_BASE 		(0x101F3000)
#define UART3_BASE 		(0x10009000)

/**********************************************/

#ifndef __ASM__

#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#define __USED		__attribute__((used))
#define __UNUSED	__attribute__((unused))
#define __SECTION(x)	__attribute__((section(x)))
#define __WEAK		__attribute__((weak))

#define ALIGN(n)	__attribute__((aligned(n)))
#define ALIGN_SIZE(n, align) ((n + align - 1) & ~(align - 1))
#define ALIGN_DOWN(n, align) ( n & ~(align - 1))

typedef int32_t		s32;
typedef int16_t		s16;
typedef int8_t		s8;
typedef uint32_t	u32;
typedef uint16_t	u16;
typedef uint8_t		u8;
typedef unsigned long ulong;

extern void printk(const char *fmt, ...);

extern void irq_enable(void);
extern void irq_disable(void);
extern uint32_t irq_lock_save(void);
extern void irq_unlock_restore(uint32_t flags);

extern void timer_irq_handler(void *data);
extern void bsp_timer_init(uint32_t index);
extern void bsp_timer_start(uint32_t index);
extern void bsp_timer_stop(uint32_t index);
extern void bsp_timer_app_init(void);

#define __raw_writeb(v, a)	(*(volatile unsigned char *)(unsigned long)(a) = (v))
#define __raw_writew(v, a)	(*(volatile unsigned short *)(unsigned long)(a) = (v))
#define __raw_writel(v, a)	(*(volatile uint32_t *)(unsigned long)(a) = (v))

#define __raw_readb(a)		(*(volatile unsigned char *)(unsigned long)(a))
#define __raw_readw(a)		(*(volatile unsigned short *)(unsigned long)(a))
#define __raw_readl(a)		(*(volatile uint32_t *)(unsigned long)(a))

#define writeb(p, v)		__raw_writeb(v, p)
#define writew(p, v)		__raw_writew(v, p)
#define writel(p, v)		__raw_writel(v, p)

#define readb(p)			__raw_readb(p)
#define readw(p)			__raw_readw(p)
#define readl(p)			__raw_readl(p)

#define setbitsb(p, mask)	writeb((p),(readb(p) | (mask)))
#define clrbitsb(p, mask)	writeb((p),(readb(p) & ~(mask)))
#define setbitsw(p, mask)	writew((p),(readw(p) | (mask)))
#define clrbitsw(p, mask)	writew((p),(readw(p) & ~(mask)))
#define setbitsl(p, mask)	writel((p),(readl(p) | (mask)))
#define clrbitsl(p, mask)	writel((p),(readl(p) & ~(mask)))

/*-----------------------irq------------------------------*/
typedef void (*irq_handler_t)(void *);
struct irq_manager_st {
	uint32_t irq;
	uint32_t type;
	void *data;
	irq_handler_t handler;
	const char *name;
};
void request_irq(uint32_t irq, uint32_t type, irq_handler_t handler,
					 void *data, const char *name);

/*---------------------------------------------------------------------------*/
#define TASK_NAME_SIZE (15)
struct task {
	void *tsp;
	void *args;
	void *stack_base;
	uint32_t stack_size;
	char name[TASK_NAME_SIZE + 1];
	void (*entry)(void *args);
};

int mx_task_init(struct task *taskobj,
	void (*entry_func)(void *args), void *args, void *stkbase, 
	uint32_t stksz, const char *name);

#endif

#endif