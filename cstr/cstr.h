#ifndef CSTR_H
#define CSTR_H

#include <stddef.h>   /* for size_t */

/*
 * Four tiny functions. All pure pointer-walking -- the exact muscle the
 * malloc macros (HDRP, FTRP, NEXT_BLKP) need. Implement each in cstr.c
 * until cstr_test.c goes green.
 *
 * Do NOT use the real string.h versions. The point is to walk the memory
 * yourself.
 */

/* Count the bytes in s before the terminating '\0'. Walk a pointer forward
 * until you hit '\0', counting steps. Do not count the '\0' itself. */
size_t my_strlen(const char *s);

/* Copy the string at src (including its '\0') into dst. Walk both pointers
 * forward, copying byte by byte, until you have copied the '\0'.
 * Returns dst. Assume dst is big enough. */
char *my_strcpy(char *dst, const char *src);

/* Copy exactly n bytes from src to dst (raw bytes, not a string -- no '\0'
 * involved). Returns dst. */
void *my_memcpy(void *dst, const void *src, size_t n);

/* Compare two strings. Walk both pointers in parallel. Return 0 if equal,
 * a negative value if a < b, positive if a > b (compare the first byte that
 * differs, as unsigned char). */
int my_strcmp(const char *a, const char *b);

#endif /* CSTR_H */
