#include "run.h"

typedef void info_handler(int, siginfo_t *, void *);

//referred online - Generalized signal transfer system
info_handler* signalTransfer(int signum, info_handler* handler) {
    struct sigaction newsig, oldsig;
    memset(&newsig, 0, sizeof(struct sigaction));
    newsig.sa_sigaction = handler;  
    sigemptyset(&newsig.sa_mask); // block sigs of type being handled
    newsig.sa_flags = SA_RESTART|SA_SIGINFO; // resume syscalls if possible

    if (sigaction(signum, &newsig, &oldsig) < 0){
        perror("shash: Sigaction Error");
    }
    return oldsig.sa_sigaction;
}

//Child exit handling
static void childOut(int sig, siginfo_t* info, void* doesntmatter) {
    pid_t pid;
    int status;
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
    {
        //Remove job from background jobs list
        for(int i = 0; i < bg_pending; i++){
            if(bg_jobs_pid[i] == pid){

                if(WEXITSTATUS(status) == 0 && WIFEXITED(status)){
                    fprintf(stderr,"%s with pid %d exited normally\n", bg_jobs_name[i], pid);
                }
                else{
                    fprintf(stderr,"%s with pid %d didn't exit normally\n", bg_jobs_name[i], pid);
                }
                //Move suffix back 1 step
                removeJob(i);

                break;
            }
        }
    }
}

//Zombie child handling
void handleZombies() {
    signalTransfer(SIGCHLD, childOut);
} 

void extrun(int num_args, char **args, int isbg){
    handleZombies();
    
    //https://www.gnu.org/software/libc/manual/html_node/Launching-Jobs.html
    pid_t pid = fork();
    if(pid < 0){
        perror("shash: Couldn't fork");
        return;
    }   
    //child
    else if(pid==0){
        signal(SIGINT, SIG_DFL); //ctrl C should work for child
        signal(SIGTSTP, SIG_DFL); //ctrl Z should work for child
        setpgid(0, 0); //tie child to new process group
        if (execvp(args[0], args) < 0) 
        {
            perror("shash: Invalid command");
            exit(EXIT_FAILURE);
        }
    }
    //parent
    else{
        int status;
        //background process handling
        if(isbg){
            printf("%d created!\n", pid);
            setpgid(pid, 0); //to avoid racecondition
            tcsetpgrp(STDIN_FILENO, getpgrp());
            //update to bg_jobs
            insertJob(bg_pending, pid, args[0]);
        }
        //foreground process handling
        else {
            //give control
            signal(SIGTTIN, SIG_IGN);   signal(SIGTTOU, SIG_IGN);
            setpgid(pid, 0);
            tcsetpgrp(STDIN_FILENO, __getpgid(pid));
            
            waitpid(pid, &status, WUNTRACED); //https://stackoverflow.com/questions/33508997/waitpid-wnohang-wuntraced-how-do-i-use-these/34845669

            //regain control
            tcsetpgrp(STDIN_FILENO, getpgrp());
            signal(SIGTTIN, SIG_DFL);   signal(SIGTTOU, SIG_DFL);

            //handle ctrl+Z on process
            if(WIFSTOPPED(status))
            {
                printf("%s with pid %d suspended\n", args[0], pid);
                insertJob(bg_pending, pid, args[0]);
            }
        }
    }
}