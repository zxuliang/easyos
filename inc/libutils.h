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

#define container_of(ptr, type, member)		\
	(type *)((char *)ptr - (char *)&(((type *)0)->member))

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

#define readb(p)		__raw_readb(p)
#define readw(p)		__raw_readw(p)
#define readl(p)		__raw_readl(p)

#define setbitsb(p, mask)	writeb((p),(readb(p) | (mask)))
#define clrbitsb(p, mask)	writeb((p),(readb(p) & ~(mask)))
#define setbitsw(p, mask)	writew((p),(readw(p) | (mask)))
#define clrbitsw(p, mask)	writew((p),(readw(p) & ~(mask)))
#define setbitsl(p, mask)	writel((p),(readl(p) | (mask)))
#define clrbitsl(p, mask)	writel((p),(readl(p) & ~(mask)))

#define isdigit(c)	((unsigned)((c) - '0') < 10)
#define TOLOWER(x)	((x) | 0x20)
#define isxdigit(c)	(('0' <= (c) && (c) <= '9') || ('a' <= (c) && (c) <= 'f') || ('A' <= (c) && (c) <= 'F'))

char *strncpy(char *dest, const char *src, size_t n);
char *strstr(const char *s1, const char *s2);
int strcasecmp(const char *a, const char *b);
int strncmp(const char *cs, const char *ct, uint32_t count);
int strcmp(const char *cs, const char *ct);
size_t strnlen(const char *s, uint32_t maxlen);
size_t strlen(const char *s);

void memset(void *base, char type, uint32_t size);
int memcmp(const void *cs, const void *ct, uint32_t count);
void *memmove(void *dest, const void *src, uint32_t n);
void *memcpy(void *dst, const void *src, uint32_t count);

int atoi(const char *nptr);
unsigned long strtoul(const char *cp,char **endp,unsigned int base);
long strtol(const char *cp, char **endp, unsigned int base);

/*------------------list------------------------*/
struct list_head
{
	struct list_head *prev;
	struct list_head *next;
};

static void xlist_add(struct list_head *new, struct list_head *prev,
		struct list_head *next)
{
	prev->next = new;
	new->prev = prev;
	new->next = next;
	next->prev = new;
}

static void xlist_del(struct list_head *prev, struct list_head *next)
{
	prev->next = next;
	next->prev = prev;
}

static void inline INIT_LIST_HEAD(struct list_head *head)
{
	head->prev = head;
	head->next = head;
}

static void inline list_add_tail(struct list_head *new, struct list_head *head)
{
	xlist_add(new, head->prev, head);
}

static void inline list_add(struct list_head *new, struct list_head *head)
{
	xlist_add(new, head, head->next);
}

static void inline list_del(struct list_head *entry)
{
	xlist_del(entry->prev, entry->next);
}

static int inline list_empty(struct list_head *entry)
{
	return entry->prev == entry ? 1 : 0;
}

static inline void list_del_init(struct list_head *entry)
{
	list_del(entry);
	INIT_LIST_HEAD(entry);
}
static inline void list_move(struct list_head *new, struct list_head *head)
{
	list_del(new);
	list_add(new, head);
}

static inline void list_move_tail(struct list_head *new,
				  struct list_head *head)
{
	list_del(new);
	list_add_tail(new, head);
}

static void inline xlist_splice(struct list_head *list,
	struct list_head *prev,
	struct list_head *next)
{
	struct list_head *first = list->next;
	struct list_head *last = list->prev;

	first->prev = prev;
	prev->next = first;

	last->next = next;
	next->prev = last;
}

static inline void list_splice(struct list_head *list,
		struct list_head *head)
{
	if (!list_empty(list))
		xlist_splice(list, head, head->next);
}

static inline void list_splice_tail(struct list_head *list,
				struct list_head *head)
{
	if (!list_empty(list))
		xlist_splice(list, head->prev, head);
}

static inline void list_replace(struct list_head *old,
				struct list_head *new)
{
	new->next = old->next;
	new->next->prev = new;
	new->prev = old->prev;
	new->prev->next = new;
}

static inline void list_replace_init(struct list_head *old,
					struct list_head *new)
{
	list_replace(old, new);
	INIT_LIST_HEAD(old);
}


#define list_entry(ptr, type, member)    \
	container_of(ptr, type, member)

#define list_first_entry(head, type, member) \
	list_entry((head)->next, type, member)

#define list_last_entry(head, type, member) \
	list_entry((head)->prev, type, member)

#define list_next_entry(pos, member) \
	list_entry((pos)->member.next, __typeof__(*(pos)), member)

#define list_prev_entry(pos, member) \
	list_entry((pos)->member.prev, __typeof__(*(pos)), member)

#define LIST_HEAD_INIT(name) { &(name), &(name) }

#define LIST_HEAD(name)	\
	struct list_head name = LIST_HEAD_INIT(name)

#define list_for_each(pos, head)	\
	for (pos = (head)->next; pos != (head); pos = pos->next)

#define list_for_each_safe(pos, n, head)				\
	for (pos = (head)->next, n = pos->next; pos != (head);		\
			pos = n, n = pos->next)

#define list_for_each_entry(pos, head, member)				\
	for (pos = list_first_entry(head, __typeof__(*(pos)), member);	\
			&pos->member != head;				\
			pos = list_next_entry(pos, member))

#define list_for_each_entry_safe(pos, n, head, member)			\
	for (pos = list_first_entry(head, typeof(*pos), member),	\
			n = list_next_entry(pos, member);		\
			&pos->member != (head); 			\
			pos = n, n = list_next_entry(n, member))

#define list_for_each_entry_reverse(pos, head, member)			\
	for (pos = list_last_entry(head, typeof(*pos), member);		\
			&pos->member != (head);					\
			pos = list_prev_entry(pos, member))







#endif