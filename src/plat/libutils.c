#include <easyos.h>

char *strncpy(char *dest, const char *src, size_t n)
{
	size_t i;

	for (i = 0; i < n && src[i] != '\0'; i++)
	   dest[i] = src[i];
	for ( ; i < n; i++)
	   dest[i] = '\0';

	return dest;
}

size_t strlen(const char *s)
{
	const char *sc;

	for (sc = s; *sc != '\0'; ++sc) /* nothing */
		;

	return sc - s;
}

size_t strnlen(const char *s, uint32_t maxlen)
{
	const char *sc;

	for (sc = s; *sc != '\0' && (uint32_t)(sc - s) < maxlen; ++sc) /* nothing */
		;

	return sc - s;
}

int strcmp(const char *cs, const char *ct)
{
	while (*cs && *cs == *ct)
		cs++, ct++;

	return (*cs - *ct);
}

int strncmp(const char *cs, const char *ct, uint32_t count)
{
	register signed char __res = 0;

	while (count)
	{
		if ((__res = *cs - *ct++) != 0 || !*cs++)
			break;
		count --;
	}

	return __res;
}

int strcasecmp(const char *a, const char *b)
{
	int ca, cb;

	do
	{
		ca = *a++ & 0xff;
		cb = *b++ & 0xff;
		if (ca >= 'A' && ca <= 'Z')
			ca += 'a' - 'A';
		if (cb >= 'A' && cb <= 'Z')
			cb += 'a' - 'A';
	}
	while (ca == cb && ca != '\0');

	return ca - cb;
}

char *strstr(const char *s1, const char *s2)
{
	int l1, l2;

	l2 = strlen(s2);
	if (!l2)
		return (char *)s1;
	l1 = strlen(s1);
	while (l1 >= l2)
	{
		l1 --;
		if (!memcmp(s1, s2, l2))
			return (char *)s1;
		s1 ++;
	}

	return NULL;
}


void memset(void *base, char type, uint32_t size)
{
	uint32_t i = 0;
	for (i = 0; i < size; i++) {
		((char *)base)[i] = type;
	}
}

void *memcpy(void *dst, const void *src, uint32_t count)
{
	char *tmp = (char *)dst, *s = (char *)src;
	uint32_t len;

	if (tmp <= s || tmp > (s + count))
	{
		while (count--)
			*tmp ++ = *s ++;
	}
	else
	{
		for (len = count; len > 0; len --)
			tmp[len - 1] = s[len - 1];
	}

	return dst;
}

void *memmove(void *dest, const void *src, uint32_t n)
{
	char *tmp = (char *)dest, *s = (char *)src;

	if (s < tmp && tmp < s + n)
	{
		tmp += n;
		s += n;

		while (n--)
			*(--tmp) = *(--s);
	}
	else
	{
		while (n--)
			*tmp++ = *s++;
	}

	return dest;
}

int memcmp(const void *cs, const void *ct, uint32_t count)
{
	const unsigned char *su1, *su2;
	int res = 0;

	for (su1 = cs, su2 = ct; 0 < count; ++su1, ++su2, count--)
		if ((res = *su1 - *su2) != 0)
			break;

	return res;
}

void *memchr(const void *s, int c, unsigned int n)
{
	const unsigned char *p = s;
	while (n-- != 0) {
		if ((unsigned char)c == *p++) {
			return (void *)(p-1);
		}
	}
	return NULL;
}

unsigned long strtoul(const char *cp,char **endp,unsigned int base)
{
	unsigned long result = 0,value;

	if (!base) {
		base = 10;
		if (*cp == '0') {
			base = 8;
			cp++;
			if ((TOLOWER(*cp) == 'x') && isxdigit(cp[1])) {
				cp++;
				base = 16;
			}
		}
	} else if (base == 16) {
		if (cp[0] == '0' && TOLOWER(cp[1]) == 'x')
			cp += 2;
	}
	while (isxdigit(*cp) &&
		(value = isdigit(*cp) ? *cp - '0' : TOLOWER(*cp)-'a'+10) < base) {
		result = result*base + value;
		cp++;
	}

	if (endp)
		*endp = (char *)cp;

	return result;
}

long strtol(const char *cp, char **endp, unsigned int base)
{
	if(*cp=='-') {
		return -strtoul(cp+1,endp,base);
	}

	return strtoul(cp,endp,base);
}

int atoi(const char *nptr)
{
	return  strtol(nptr, (char  **)NULL, 10);
}
