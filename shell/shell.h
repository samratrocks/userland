#ifndef SHELL_H
#define SHELL_H

/*
 * Parse a command line into NULL-terminated argv.
 * Splits on whitespace. Returns argc; argv[argc] == NULL.
 * Modifies line in place.
 */
int parse(char *line, char **argv, int max);

/*
 * Fork, exec argv in the child, wait in the parent.
 * Returns the child's exit status, or -1 on fork/exec failure.
 */
int run(char **argv);

#endif
