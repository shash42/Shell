#ifndef __LS_H
#define __LS_H
#include <time.h>
#include "headers.h"
#include "directory.h"

int hide_filter(const struct dirent *entry);
int lslong(char *actpath, char *actname);
int execls(char *dirpath, int aflag, int lflag);
int ls(int num_args, char **args);

#endif