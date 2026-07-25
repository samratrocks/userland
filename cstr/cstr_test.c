/*
 * cstr_test.c -- tests for the four pointer-walking warmups.
 *
 *   cc -Wall -o cstr_test cstr.c cstr_test.c
 *   ./cstr_test
 *
 * assert() aborts on the first failure, so fix one at a time.
 * Order in main(): strlen, strcpy, memcpy, strcmp.
 */

#include "cstr.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>   /* only for comparing against the real versions */

/* ---- my_strlen ---- */
static void
test_strlen(void)
{
	assert(my_strlen("") == 0);
	assert(my_strlen("a") == 1);
	assert(my_strlen("hello") == 5);
	assert(my_strlen("with spaces too") == 15);
	printf("  ok    my_strlen\n");
}

/* ---- my_strcpy ---- */
static void
test_strcpy(void)
{
	char buf[64];

	assert(my_strcpy(buf, "hello") == buf);   /* returns dst */
	assert(strcmp(buf, "hello") == 0);        /* content matches */

	my_strcpy(buf, "");
	assert(buf[0] == '\0');                   /* empty string -> just the '\0' */

	my_strcpy(buf, "a longer string here");
	assert(strcmp(buf, "a longer string here") == 0);
	printf("  ok    my_strcpy\n");
}

/* ---- my_memcpy ---- */
static void
test_memcpy(void)
{
	char src[] = {1, 2, 3, 4, 5};
	char dst[5] = {0};

	assert(my_memcpy(dst, src, 5) == dst);    /* returns dst */
	for (int i = 0; i < 5; i++)
		assert(dst[i] == src[i]);

	/* n == 0 should copy nothing and not crash */
	char x = 7;
	my_memcpy(&x, "Z", 0);
	assert(x == 7);

	/* raw bytes, including a zero in the middle -- not a string copy */
	char rsrc[] = {9, 0, 9};
	char rdst[3] = {0};
	my_memcpy(rdst, rsrc, 3);
	assert(rdst[0] == 9 && rdst[1] == 0 && rdst[2] == 9);
	printf("  ok    my_memcpy\n");
}

/* ---- my_strcmp ---- */
static void
test_strcmp(void)
{
	assert(my_strcmp("abc", "abc") == 0);     /* equal */
	assert(my_strcmp("", "") == 0);
	assert(my_strcmp("abc", "abd") < 0);      /* a < b */
	assert(my_strcmp("abd", "abc") > 0);      /* a > b */
	assert(my_strcmp("ab", "abc") < 0);       /* prefix is smaller */
	assert(my_strcmp("abc", "ab") > 0);
	printf("  ok    my_strcmp\n");
}

int
main(void)
{
	test_strlen();
	test_strcpy();
	test_memcpy();
	test_strcmp();
	printf("all passed\n");
	return (0);
}
