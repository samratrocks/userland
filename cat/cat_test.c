/*
 * cat_test.c -- tests for cat_fd() and cat_path()
 *
 *   cc -Wall -o cat_test cat.c cat_test.c
 *   ./cat_test
 *
 * assert() aborts on the first failure, so you fix one test at a time.
 * That is the intended rhythm: red, green, next.
 */

#include "cat.h"

#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define CAP 262144

/*
 * Make a temp file holding n bytes, rewound to offset 0.
 * The name is unlinked immediately -- the descriptor keeps the file
 * alive, and it disappears when we close it. No cleanup needed.
 */
static int
tmp_with(const void *data, size_t n)
{
	char tmpl[] = "/tmp/catXXXXXX";
	int fd = mkstemp(tmpl);

	assert(fd >= 0);
	unlink(tmpl);
	if (n > 0)
		assert(write(fd, data, n) == (ssize_t)n);
	assert(lseek(fd, 0, SEEK_SET) == 0);
	return (fd);
}

/*
 * Like tmp_with, but leaves the file on disk and hands back the path
 * so cat_path() can open it by name. Caller unlinks when done.
 */
static void
tmp_named(const void *data, size_t n, char *path_out, size_t path_cap)
{
	char tmpl[] = "/tmp/catpathXXXXXX";
	int fd = mkstemp(tmpl);

	assert(fd >= 0);
	if (n > 0)
		assert(write(fd, data, n) == (ssize_t)n);
	assert(close(fd) == 0);
	assert(strlen(tmpl) < path_cap);
	strcpy(path_out, tmpl);
}

/*
 * Read the whole file back. One read() is enough here: these are small
 * regular files, and a regular-file read returns everything you ask for
 * short of EOF. Your cat_fd() cannot assume that -- it may be handed a
 * pipe or a terminal.
 */
static size_t
slurp(int fd, void *out, size_t cap)
{
	ssize_t n;

	assert(lseek(fd, 0, SEEK_SET) == 0);
	n = read(fd, out, cap);
	assert(n >= 0);
	return ((size_t)n);
}

/* Varying, non-repeating-on-buffer-boundaries filler. */
static void
fill(unsigned char *p, size_t n)
{
	size_t i;

	for (i = 0; i < n; i++)
		p[i] = (unsigned char)(i % 251);
}

/* ---- cat_fd ---- */

/*
 * The shape of every cat_fd test: bytes in, cat_fd, bytes out, compare.
 * Checks three things -- return value, byte count, byte content.
 */
static void
roundtrip(const void *data, size_t n, const char *name)
{
	static unsigned char got[CAP];
	size_t m;
	int in, out, rc;

	in = tmp_with(data, n);
	out = tmp_with(NULL, 0);

	rc = cat_fd(in, out);
	assert(rc == 0);

	m = slurp(out, got, sizeof got);
	assert(m == n);
	assert(memcmp(got, data, n) == 0);

	close(in);
	close(out);
	printf("  ok    %s\n", name);
}

static void
test_empty(void)
{
	roundtrip("", 0, "fd: empty input produces empty output");
}

static void
test_short(void)
{
	roundtrip("hello", 5, "fd: five bytes arrive intact");
}

/*
 * 100003 is prime and larger than any sane buffer, so no buffer size
 * divides it evenly and no single read() can swallow it. Fails if you
 * did not loop, and fails if you wrote sizeof(buf) instead of the
 * count read() handed back.
 */
static void
test_larger_than_buffer(void)
{
	static unsigned char data[100003];

	fill(data, sizeof data);
	roundtrip(data, sizeof data, "fd: 100003 bytes -- forces the loop");
}

/* Same trap, small enough to fail fast while you are iterating. */
static void
test_ragged(void)
{
	static unsigned char data[4999];

	fill(data, sizeof data);
	roundtrip(data, sizeof data, "fd: 4999 bytes -- forces the right count");
}

static void
test_bad_input_fd(void)
{
	int out = tmp_with(NULL, 0);

	assert(cat_fd(-1, out) == -1);
	close(out);
	printf("  ok    fd: bad input descriptor returns -1\n");
}

/* ---- cat_path ---- */

/*
 * cat_path opens the path itself, copies, and closes its own descriptor.
 * We give it a real file on disk and check the bytes land in out_fd.
 */
static void
path_roundtrip(const void *data, size_t n, const char *name)
{
	static unsigned char got[CAP];
	char path[64];
	size_t m;
	int out, rc;

	tmp_named(data, n, path, sizeof path);
	out = tmp_with(NULL, 0);

	rc = cat_path(path, out);
	assert(rc == 0);

	m = slurp(out, got, sizeof got);
	assert(m == n);
	assert(memcmp(got, data, n) == 0);

	unlink(path);
	close(out);
	printf("  ok    %s\n", name);
}

static void
test_path_short(void)
{
	path_roundtrip("hello", 5, "path: five bytes arrive intact");
}

static void
test_path_larger_than_buffer(void)
{
	static unsigned char data[100003];

	fill(data, sizeof data);
	path_roundtrip(data, sizeof data, "path: 100003 bytes -- opens, loops, closes");
}

/*
 * A path that does not exist. open() fails, and cat_path must surface
 * that as a negative return rather than pressing on.
 */
static void
test_path_missing(void)
{
	int out = tmp_with(NULL, 0);

	assert(cat_path("/tmp/definitely-not-a-real-file-xyzzy-42", out) < 0);
	close(out);
	printf("  ok    path: missing file returns negative\n");
}

int
main(void)
{
	printf("cat_fd\n");
	test_empty();
	test_short();
	test_larger_than_buffer();
	test_ragged();
	test_bad_input_fd();

	printf("cat_path\n");
	test_path_short();
	test_path_larger_than_buffer();
	test_path_missing();

	printf("all passed\n");
	return (0);
}
