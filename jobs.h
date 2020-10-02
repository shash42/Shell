#ifndef __JOBS_H
#define __JOBS_H
#include "headers.h"
#include "directory.h"

int jobs(int num_args, char **args); //implement jobs command
int kjob(int num_args, char **args); //implement kjob command
int overkill(int num_args, char **args); //implement overkill command
int bg(int num_args, char **args); //implement bg command
int fg(int num_args, char **args); //implement fg command
void removeJob(int i); //remove ith job from bg list
void insertJob(int i, int pid, char *jobname); //insert job to ith posn in bg list

#endif