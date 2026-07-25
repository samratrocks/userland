#include "shell.h"

#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

/*
 * Split line into argv on whitespace (spaces and tabs).
 * NULL-terminate argv, return argc. Blank line returns 0.
 */
int
parse(char *line, char **argv, int max)
{
	int argc = 0;

	while (*line != '\0') {
		while (*line == ' ' || *line == '\t')
			line++;

		if (*line == '\0')
			break;

		if (argc >= max - 1)
			break;

		argv[argc++] = line;

		while (*line != '\0' && *line != ' ' && *line != '\t')
			line++;

		if (*line != '\0')
			*line++ = '\0';
	}

	argv[argc] = NULL;
	return (argc);
}
/*
 * Fork, exec argv in the child, wait for it in the parent.
 * Return the child's exit status, or -1 if fork or exec fails.
 * If exec fails, _exit the child -- do not return (fork bomb).
 */
int
run(char **argv)
{
	/* Get the first item from argv, and give that to the kernel to run it. */

	pid_t pid;
	int status;

	pid = fork();   /* I also need to provide some arguments */
	switch (pid) {
	case -1:
	    return -1;
	case 0:
	    execv(argv[0], argv); /* how do i get path? */
		_exit(127);
	default:
	    waitpid(pid, &status, 0); /* whatevedr is the process */
		return WEXITSTATUS(status);
	}

	return (-1);
}
