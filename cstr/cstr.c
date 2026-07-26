#include "cstr.h"

/* Walk s forward counting bytes until '\0'. */
size_t
my_strlen(const char *s)
{
	size_t size = 0;

	while (*s != '\0') {
		s++;
		size++;
	}

	return (size);
}

/* Copy src into dst byte by byte, including the '\0'. Return dst. */
char *
my_strcpy(char *dst, const char *src)
{
	char *start = dst;
	while (*src != '\0')
	{
		*dst = *src;
		src++;
		dst++;
	}

	*dst = '\0';

	return (start);
}

/* Copy exactly n raw bytes from src to dst. Return dst. */
void *
my_memcpy(void *dst, const void *src, size_t n)
{
	char *start = dst;
	char *d = dst;
	const char *s = src;
	size_t size = 0;
	
	while (size < n)
	{
		*d = *s;
		s++;
		d++;
		size++;
	}
	return (start);
}

/* Compare a and b byte by byte. 0 if equal, sign of first difference else. */
int
my_strcmp(const char *a, const char *b)
{
	while (*a != '\0' && *a == *b)
	{
		a++;
		b++;
	}
	return ((unsigned char) *a - (unsigned char) *b);
}
 