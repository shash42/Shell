#ifndef __DIRECTORY_H
#define __DIRECTORY_H

#include "headers.h"

char *readFileLine(char *path, int line_num);
int isPrefix(int home_len, int cwd_len, char *curr);
char *getShellDir(char *curr);
char *getRootDir(char *inppath);
void cd(int num_args, char **args);
void pwd(int num_args, char **args);
#endif