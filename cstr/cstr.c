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
	/* Iterate one at a time
	 * set the value to the dereference operations
	 * once it works for one it should work for all
	*/
	char *start = dst;
	while (*src != '\0')
	{
		*dst = *src;	/* Here im assinging the value of src to dst */
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
	return (dst);
}

/* Compare a and b byte by byte. 0 if equal, sign of first difference else. */
int
my_strcmp(const char *a, const char *b)
{
	return (0);
}
 