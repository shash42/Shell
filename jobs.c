#include "jobs.h"

//remove ith job
void removeJob(int i){
    for(int j = i; j < bg_pending - 1; j++){
        bg_jobs_pid[j] = bg_jobs_pid[j+1];
        strcpy(bg_jobs_name[j], bg_jobs_name[j+1]);
    }
    bg_pending--;
}

//add job in ith position
void insertJob(int i, int pid, char *jobname){
    for(int j = bg_pending; j > i; j--){
        bg_jobs_pid[j] = bg_jobs_pid[j-1];
        strcpy(bg_jobs_name[j], bg_jobs_name[j-1]);
    }
    bg_jobs_pid[i] = pid;
    strcpy(bg_jobs_name[i], jobname);
    bg_pending++;
}

int jobs(int num_args, char **args){
    /*if(num_args>1){
        fprintf(stderr, "shash: jobs: incorrect number of arguments, 0 required\n");
        return;
    }*/
    for(int i = 0; i < bg_pending; i++){
        //Get status
        char path[50], status[50];
        sprintf(path, "/proc/%d/status", bg_jobs_pid[i]);
        strcpy(status, "Running");
        char *statline = readFileLine(path, 3); strtok(statline, " :\t");
        char *sinfo = strtok(NULL, " :\t");
        if(strcmp(sinfo, "R")!=0){
            strcpy(status, "Stopped");
        }
        
        //Print process
        printf("[%d] %s %s [%d]\n", i+1, status, bg_jobs_name[i], bg_jobs_pid[i]);
        free(statline);
    }
    return 0;
}

int kjob(int num_args, char **args){
    //Initial processing of args
    if(num_args!=3){
        fprintf(stderr, "shash: kjob: incorrect number of arguments. Usage: kjob <job number> <signal number>\n");
        return 1;
    }
    int pididx = atoi(args[1]);
    if(pididx > bg_pending || pididx<=0){
        fprintf(stderr, "shash: kjob: invalid process number specified, 1 to %d active\n", bg_pending);
        return 1;
    }
    int pid = bg_jobs_pid[pididx-1];

    //Send signal to job
    int ret = kill(pid, atoi(args[2]));
    if(ret < 0){
        perror("shash: kjob");
        return 1;
    }
    return 0;
}

int overkill(int num_args, char **args){
    for(int i = 0; i < bg_pending; i++){
        kill(bg_jobs_pid[i], SIGKILL); //murder!
    }
    return 0;
}

int bg(int num_args, char **args){
    //Initial processing of args
    if(num_args!=2){
        fprintf(stderr, "shash: bg: incorrect number of arguments. Usage: bg <job number>\n");
        return 1;
    }
    int pididx = atoi(args[1]);
    if(pididx > bg_pending || pididx<=0){
        fprintf(stderr, "shash: bg: invalid process number specified, 1 to %d active\n", bg_pending);
        return 1;
    }
    int pid = bg_jobs_pid[pididx-1];

    //send continue signal to pid
    int ret = kill(pid, SIGCONT);
    if(ret < 0){
        perror("shash: bg");
        return 1;
    }   
    return 0;
}

int fg(int num_args, char **args){
    //Initial processing of args
    if(num_args!=2){
        fprintf(stderr, "shash: fg: incorrect number of arguments. Usage: fg <job number>\n");
        return 1;
    }
    int pididx = atoi(args[1]);
    if(pididx > bg_pending || pididx<=0){
        fprintf(stderr, "shash: fg: invalid process number specified, 1 to %d active\n", bg_pending);
        return 1;
    }
    int pid = bg_jobs_pid[pididx-1];

    //Bring pid to fg
    //give control
    char *savename = (char *)malloc(ARG_LEN_MAX);
    strcpy(savename, bg_jobs_name[pididx-1]);
    removeJob(pididx-1); //no more a bg process
    signal(SIGTTIN, SIG_IGN);   signal(SIGTTOU, SIG_IGN);
    setpgid(pid, 0);
    tcsetpgrp(STDIN_FILENO, __getpgid(pid));
    kill(pid, SIGCONT); //continues if stopped

    int status;
    waitpid(pid, &status, WUNTRACED); //https://stackoverflow.com/questions/33508997/waitpid-wnohang-wuntraced-how-do-i-use-these/34845669

    //regain control
    tcsetpgrp(STDIN_FILENO, getpgrp());
    signal(SIGTTIN, SIG_DFL);   signal(SIGTTOU, SIG_DFL);

    if(WIFSTOPPED(status)) //handle ctrl+Z
	{
		printf("%s with pid %d suspended\n", savename, pid);
		insertJob(pididx-1, pid, savename);
	}
    free(savename);

    if(status > 0){
        return 1;
    }
    return 0;
}