/*
 * shell_test.c -- tests for stage 1 (parse + run)
 *
 *   cc -Wall -o shell_test shell.c shell_test.c
 *   ./shell_test
 *
 * assert() aborts on the first failure, so you fix one at a time.
 *
 * parse() is pure string work and easy to test exhaustively.
 * run() forks and execs a real program, so its tests shell out to
 * /bin/true, /bin/false, and /bin/echo and check the exit status.
 */

#include "shell.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

#define MAXARGS 64

/* ---- parse ---- */

/*
 * Helper: parse a copy of `line` (parse modifies in place) and assert
 * the resulting argv matches the NULL-terminated `want` array exactly,
 * including the count and the terminating NULL.
 */
static void
check_parse(const char *line, const char *const *want)
{
	char buf[256];
	char *argv[MAXARGS];
	int argc, i;

	assert(strlen(line) < sizeof buf);
	strcpy(buf, line);

	argc = parse(buf, argv, MAXARGS);

	/* count how many the caller expected */
	for (i = 0; want[i] != NULL; i++)
		;
	assert(argc == i);

	/* every slot matches */
	for (i = 0; i < argc; i++)
		assert(strcmp(argv[i], want[i]) == 0);

	/* argv is NULL-terminated at argc -- this is what exec needs */
	assert(argv[argc] == NULL);
}

static void
test_parse_single(void)
{
	static const char *const want[] = {"ls", NULL};

	check_parse("ls", want);
	printf("  ok    parse: single word\n");
}

static void
test_parse_multiple(void)
{
	static const char *const want[] = {"ls", "-l", "-a", NULL};

	check_parse("ls -l -a", want);
	printf("  ok    parse: multiple words\n");
}

static void
test_parse_extra_spaces(void)
{
	static const char *const want[] = {"ls", "-l", NULL};

	check_parse("   ls    -l   ", want);
	printf("  ok    parse: leading/trailing/repeated spaces\n");
}

static void
test_parse_tabs(void)
{
	static const char *const want[] = {"echo", "hi", NULL};

	check_parse("echo\thi", want);
	printf("  ok    parse: tabs count as whitespace\n");
}

static void
test_parse_empty(void)
{
	static const char *const want[] = {NULL};

	check_parse("", want);
	check_parse("     ", want);
	printf("  ok    parse: blank line yields argc 0, argv[0] NULL\n");
}

/* ---- run ---- */

static void
test_run_true(void)
{
	char *argv[] = {"/bin/true", NULL};

	/* /bin/true exits 0 */
	assert(run(argv) == 0);
	printf("  ok    run: /bin/true returns 0\n");
}

static void
test_run_false(void)
{
	char *argv[] = {"/bin/false", NULL};

	/* /bin/false exits 1 */
	assert(run(argv) == 1);
	printf("  ok    run: /bin/false returns 1\n");
}

static void
test_run_echo(void)
{
	char *argv[] = {"/bin/echo", "hello", NULL};

	/* echo exits 0; its output goes to the terminal, which is fine */
	assert(run(argv) == 0);
	printf("  ok    run: /bin/echo runs and returns 0\n");
}

static void
test_run_missing(void)
{
	char *argv[] = {"/no/such/program/xyzzy", NULL};

	/* exec fails in the child -- run must report failure, not 0 */
	assert(run(argv) == 127);
	printf("  ok    run: missing program returns 127\n");
}

int
main(void)
{
	printf("parse\n");
	test_parse_single();
	test_parse_multiple();
	test_parse_extra_spaces();
	test_parse_tabs();
	test_parse_empty();

	printf("run\n");
	test_run_true();
	test_run_false();
	test_run_echo();
	test_run_missing();

	printf("all passed\n");
	return (0);
}
