#ifndef __HEADERS_H
#define __HEADERS_H

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <linux/limits.h>
#include <limits.h>
#include <sys/utsname.h>
#include <pwd.h>
#include <errno.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <grp.h>
#include <time.h>
#include <fcntl.h>

#define LINE_LEN_MAX 100005
#define ARGS_MAX 102
#define ARG_LEN_MAX 1003 //input to echo is also bounded by this
#define ECHO_LEN_MAX 1003
#define BG_JOBS_MAX 102
#define PROC_LINE_MAX 402
#define HIST_MAX 22
#define HIST_LIM 20
#define HIST_DEF 10

char home[PATH_MAX + 2], cwd[PATH_MAX+2], lwd[PATH_MAX+2];
char bg_jobs_name[BG_JOBS_MAX][ARG_LEN_MAX];
pid_t bg_jobs_pid[BG_JOBS_MAX];
int bg_pending;

#endif