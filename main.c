#include "headers.h"
#include "printprompt.h"
#include "directory.h"
#include "echo.h"
#include "ls.h"
#include "run.h"
#include "pinfo.h"
#include "history.h"
#include "nightswatch.h"
#include "env.h"
#include "jobs.h"

//Get a line of input (no processing)
char *getInput(){
    size_t inp_len = 0;
    char *line = NULL;
    int ret = getline(&line, &inp_len, stdin);
    if(ret==-1){
        printf("exit\n");
        exit(0);
    }
    return line;
}

void commanderexit(char **args, int num_args){
    for(int i = 0; i < num_args; i++){
        free(args[i]);
    }
    free(args);
}

int hasPipes(char *line){
    int cnt = 0;
    for(int i = 0; i < strlen(line); i++){
        if(line[i]=='|') cnt++;
    }
    return cnt;
}

int Piping(char *line);

void commander(char *line){
    //printf("this %s\n", line);
    char *cmd = (char *)malloc(LINE_LEN_MAX);
    strcpy(cmd, line); //isn't this line redundant?!
    //array of arguments
    char **args = (char**)malloc(sizeof(char*) * ARGS_MAX);
    int num_args = 0;
    
    //pipe-handling
    if(hasPipes(line)){
        Piping(line);
        commanderexit(args, num_args);
        return;
    }

    char redirout[ARG_LEN_MAX], redirin[ARG_LEN_MAX];
    strcpy(redirout, "\0"); strcpy(redirin, "\0"); //needed for checking len later
    
    //cmd iterates over tokens of the input line incl. redirection
    cmd = strtok(line, " \n\r\t");
    int isApp = 0;
    while(cmd!=NULL){
        //printf("%s\n", cmd);
        //Getting redirection path

        //Output
        if(strcmp(cmd, ">")==0 || strcmp(cmd, ">>")==0){
            if(strcmp(cmd, ">>")==0) isApp = 1;
            else isApp = 0; //necessary because intermediate >> dont matter, only final >
            cmd = strtok(NULL, " \n\r\t");
            strcpy(redirout, cmd);
            int fd_out = open(redirout, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if(fd_out < 0){
                perror("shash: Output Redirection");
                commanderexit(args, num_args);
                return;
            }
            close(fd_out);

            cmd = strtok(NULL, " \n\r\t");
            continue;
        }

        //Input
        else if(strcmp(cmd, "<")==0){
            cmd = strtok(NULL, " \n\r\t");
            strcpy(redirin, cmd);
            int fd_in = open(redirin, O_RDONLY);
            if(fd_in < 0){
                perror("shash: Input Redirection");
                commanderexit(args, num_args);
                return;
            }
            close(fd_in);
            
            cmd = strtok(NULL, " \n\r\t");
            continue;
        }

        if(strlen(cmd)>0){
            args[num_args] = (char *)malloc(ARG_LEN_MAX);
            strcpy(args[num_args++], cmd);
        }
        cmd = strtok(NULL, " \n\r\t");
    }
    args[num_args] = NULL;
    free(cmd);
    if(num_args<1){ //handle commands that only have spaces lol
        commanderexit(args, num_args);
        return;
    }
    //handle background job input
    int isbg = 0;
    if(strcmp(args[num_args-1], "&")==0)
    {
        args[num_args-1] = NULL;
        isbg=1;
    }
    else if(args[num_args-1][strlen(args[num_args-1]) - 1] == '&')
    {
        args[num_args-1][strlen(args[num_args-1]) -1] = '\0';
        isbg=1;
    }

    //change fds for redirection
    int newfdin = -1, newfdout = -1, iserr=0;
    int ogfdin = dup(0);    int ogfdout = dup(1); //save old fd
    if(strlen(redirin) > 0){
        newfdin = open(redirin, O_RDONLY, 0644);
        if (newfdin < 0){
            iserr = 1;
            perror("shash: Input Redirection");
        }
        else dup2(newfdin, STDIN_FILENO);
    }
    if(iserr==0 && strlen(redirout) > 0){
        if(isApp){
            newfdout = open(redirout, O_WRONLY | O_CREAT | O_APPEND, 0644); 
        }
        else {
            newfdout = open(redirout, O_WRONLY | O_CREAT | O_TRUNC, 0644); 
        }
        if(newfdout < 0){
            iserr = 1;
            perror("shash: Output Redirection");
        }
        else dup2(newfdout, STDOUT_FILENO);
    }
    if(iserr){
        commanderexit(args, num_args);
        return;
    }

    //Command matching and calling right func
    if(strcmp(args[0], "exit")==0) exit(0);

    else if(strcmp(args[0], "cd")==0){
        cd(num_args, args);
    }

    else if(strcmp(args[0], "pwd")==0){
        pwd(num_args, args);
    }

    else if(strcmp(args[0], "echo")==0){
        echo(num_args, args);
    }

    else if(strcmp(args[0], "ls")==0){
        ls(num_args, args);
    }

    else if(strcmp(args[0], "pinfo")==0){
        pinfo(num_args, args);
    }

    else if(strcmp(args[0], "history")==0){
        history(num_args, args);
    }

    else if(strcmp(args[0], "nightswatch")==0){
        nightswatch(num_args, args);
    }

    else if(strcmp(args[0], "setenv")==0){
        mysetenv(num_args, args);
    }

    else if(strcmp(args[0], "unsetenv")==0){
        myunsetenv(num_args, args);
    }

    else if(strcmp(args[0], "jobs")==0){
        jobs(num_args, args);
    }

    else if(strcmp(args[0], "kjob")==0){
        kjob(num_args, args);
    }
    
    else if(strcmp(args[0], "overkill")==0){
        overkill(num_args, args);
    }

    else if(strcmp(args[0], "quit")==0){
        printf("shash: Imma head out /;-;/\n");
        overkill(num_args, args);
        commanderexit(args, num_args);
        exit(0);
    }

    else if(strcmp(args[0], "bg")==0){
        bg(num_args, args);
    }

    else if(strcmp(args[0], "fg")==0){
        fg(num_args, args);
    }
    
    else{
        extrun(num_args, args, isbg);
    }   

    //Terminating Redirection
    if(newfdin > 0) close(newfdin);
    if(newfdout > 0) close(newfdout);
    if(strlen(redirin) > 0){
        dup2(ogfdin, STDIN_FILENO);
    }
    if(strlen(redirout) > 0){
        dup2(ogfdout, STDOUT_FILENO);
    }
    commanderexit(args, num_args);
}

void shell(){
    while(1){
        //Prompt and History logging
        getcwd(cwd, PATH_MAX + 2);
        printPrompt();
        char *longline = getInput();
        logHistory(longline);
        
        //Token on ; and \n to get indiv commands
        char *saveptr1;
        char *line = strtok_r(longline, ";\n", &saveptr1);
        //printf("thisss %s\n", line);
        while(line != NULL){
            if(strlen(line)>0){
                commander(line);
                getcwd(cwd, PATH_MAX + 2);
            }
            line = strtok_r(NULL, ";\n", &saveptr1);
        }
        free(longline);
    }
}

signed main(){
    signal(SIGINT, SIG_IGN); //ctrl C ignored by terminal
    signal(SIGTSTP, SIG_IGN); //ctrl Z ignored by terminal
    getcwd(home, PATH_MAX + 2);
    strcpy(lwd, "\0");
    bg_pending = 0;
    //printf("%s\n", home_dir);
    
    shell();
}

int Piping(char *line){
    char *curr = (char *)malloc(LINE_LEN_MAX);
    char *next = (char *)malloc(LINE_LEN_MAX);
    int pipefd[2], fdprvout = STDIN_FILENO;

    next = strtok(line, "|");
    while(next != NULL){
        strcpy(curr, next); //curr is what was next previously
        next = strtok(NULL, "|"); //shift next, needed to see if another pipe afterwards
        int ret = pipe(pipefd);
        if(ret < 0){
            perror("shash: Piping");
            return -1;
        }

        pid_t pid = fork();
        if(pid < 0){
            perror("shash: Piping pid < 0");
            return -1;
        }
        if(pid == 0){ //child
            close(pipefd[0]);
            dup2(fdprvout, STDIN_FILENO); //take input from prvout (initially stdin)
            if(next!=NULL){ //if another piped process after it
                dup2(pipefd[1], STDOUT_FILENO); //all output to pipe-writeend (otherwise to stdout)
            }
            close(pipefd[1]); //closing the FD, not the file (stored in STDOUT now)
            commander(curr); //run the current command
            exit(2);
        }
        else{ //parent
            wait(NULL); //wait for child to end
            close(pipefd[1]);
            fdprvout = pipefd[0]; //fdprvout now points to output of previous
        }
    }
    close(fdprvout);   
    free(curr); free(next);
    return 0;
}