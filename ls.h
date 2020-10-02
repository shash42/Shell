#ifndef __LS_H
#define __LS_H
#include <time.h>
#include "headers.h"
#include "directory.h"

int hide_filter(const struct dirent *entry);
void lslong(char *actpath, char *actname);
void execls(char *dirpath, int aflag, int lflag);
void ls(int num_args, char **args);

#endif