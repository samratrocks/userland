#ifndef CAT_H
#define CAT_H

#include <sys/types.h>

int cat_fd(int in_fd, int out_fd);
int cat_path(const char *path, int out_fd);

#endif
