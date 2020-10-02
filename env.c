#include "env.h"

void mysetenv(int num_args, char **args){
    if(num_args > 3 || num_args < 2){
        fprintf(stderr,"shash: setenv: incorrect no. of arguments, 1-2 required\n");
        return;
    }
    char envval[ARG_LEN_MAX];
    envval[0] = '\0';
    if(num_args==3){
        strcpy(envval, args[2]);
    }
    setenv(args[1], envval, 1);
}

void myunsetenv(int num_args, char **args){
    if(num_args != 2){
        fprintf(stderr,"shash: unsetenv: incorrect no. of arguments, 1-2 required\n");
        return;
    }
    unsetenv(args[1]);
}