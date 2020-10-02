#ifndef __JOBS_H
#define __JOBS_H
#include "headers.h"
#include "directory.h"

void jobs(int num_args, char **args); //implement jobs command
void kjob(int num_args, char **args); //implement kjob command
void overkill(int num_args, char **args); //implement overkill command
void bg(int num_args, char **args); //implement bg command
void fg(int num_args, char **args); //implement fg command
void removeJob(int i); //remove ith job from bg list
void insertJob(int i, int pid, char *jobname); //insert job to ith posn in bg list

#endif