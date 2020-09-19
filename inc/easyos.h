
#ifndef __EASY_READ_WRITE_H__
#define __EASY_READ_WRITE_H__


#ifndef __ASM__

#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#define __raw_writeb(v, a)	(*(volatile unsigned char *)(unsigned long)(a) = (v))
#define __raw_writew(v, a)	(*(volatile unsigned short *)(unsigned long)(a) = (v))
#define __raw_writel(v, a)	(*(volatile unsigned int *)(unsigned long)(a) = (v))

#define __raw_readb(a)		(*(volatile unsigned char *)(unsigned long)(a))
#define __raw_readw(a)		(*(volatile unsigned short *)(unsigned long)(a))
#define __raw_readl(a)		(*(volatile unsigned int *)(unsigned long)(a))

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

extern char console_getc(void);
extern void console_putc(const char c);
extern int printk(const char *fmt, ...);

#endif




#endif