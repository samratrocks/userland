#include <unistd.h>
#include <fcntl.h>
#include "cat.h"
/* syscalls used:
 *
 * 	ssize_t read(int fd, void *buf, size_t count);
 * 	ssize_t write(int fd, const void *buf, size_t count);
 *
 * 	int open(const char *path, int flags, ...);
 * 	int close(int fd);
 */
 
/*
 * Read from a file descriptor and pass it to another.
 */
int
cat_fd(int in_fd, int out_fd)
{
    size_t count = 5000;
    char buf[count];
    ssize_t inCount;
    while ((inCount = read(in_fd, buf, count)) > 0) {
        write(out_fd, buf, inCount);
    }
    return (inCount < 0) ? -1 : 0;
}
 
/*
 * Read from a file path and pass to a file descriptor.
 */
int
cat_path(const char* path, int out_fd)
{
    int in_fd = open(path, O_RDONLY);
    if (in_fd < 0) return in_fd;
    int val = cat_fd(in_fd, out_fd);
    int close_fd = close(in_fd);
    if (val < 0 || close_fd < 0) return (-1);
    return (0);
}
 

