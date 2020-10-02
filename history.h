#ifndef __HISTORY_H
#define __HISTORY_H

#include "headers.h"

char hist_list[HIST_MAX][LINE_LEN_MAX];
int num_hist;
void getHistory();
void writeHistory();
void logHistory(char *toadd);
int history(int num_args, char **args);

#endif