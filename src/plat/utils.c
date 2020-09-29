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

void memset(void *base, char type, uint32_t size)
{
	uint32_t i = 0;
	for (i = 0; i < size; i++) {
		((char *)base)[i] = type;
	}
}