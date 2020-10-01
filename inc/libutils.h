#ifndef __LIB_UTILS_H__
#define __LIB_UTILS_H__

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



char *strncpy(char *dest, const char *src, size_t n);
void memset(void *base, char type, uint32_t size);

#endif