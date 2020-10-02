#ifndef __DIRECTORY_H
#define __DIRECTORY_H

#include "headers.h"

char *readFileLine(char *path, int line_num);
int isPrefix(int home_len, int cwd_len, char *curr);
char *getShellDir(char *curr);
char *getRootDir(char *inppath);
int cd(int num_args, char **args);
int pwd(int num_args, char **args);
#endif