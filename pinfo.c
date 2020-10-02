#include "pinfo.h"

//Implements the pinfo command as required
void pinfo(int num_args, char **args){
    char prefix[30], path[PATH_MAX];
    int pid = getpid();
    if(num_args>2){
        fprintf(stderr, "shash: pinfo: too many arguments\n");
    }
    else if(num_args==2){ //for a particular pid
        pid = atoi(args[1]);
        sprintf(prefix, "/proc/%d/", pid);
    }
    else{ //for current pid, none specified.
        sprintf(prefix, "/proc/%d/", pid);
    }

    //setup up path and check it can be accessed
    path[0] = '\0';
    strcpy(path, prefix);
    strcat(path, "status");
    if(access(path, R_OK) == -1){
        fprintf(stderr, "shash: pinfo: (%d) - No such process\n", pid);
        return;
    }

    //extract the information required from proc/pid/status
    char *mem = readFileLine(path, 18); strtok(mem, " :\t");
    char *minfo = strtok(NULL, " :\t");
    char *status = readFileLine(path, 3); strtok(status, " :\t");
    char *sinfo = strtok(NULL, " :\t");

    //extract executable path from proc/pid/exes
    strcpy(path, prefix);
    strcat(path, "exe");
    char exec_path[PATH_MAX];
    int ret = readlink(path, exec_path, PATH_MAX - 1);
    exec_path[ret] = '\0';
    char *shell_dir = getShellDir(exec_path);
    strcpy(exec_path, shell_dir);
    free(shell_dir);
    //print in reqd format
    printf("pid -- %d\nProcess Status -- %s\n", pid, sinfo); 
    printf("Virtual Memory -- %s\nExecutable path -- %s\n", minfo, exec_path);

    free(mem); free(status);
}