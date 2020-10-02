#include "env.h"

int mysetenv(int num_args, char **args){
    if(num_args > 3 || num_args < 2){
        fprintf(stderr,"shash: setenv: incorrect no. of arguments, 1-2 required\n");
        return 1;
    }
    char envval[ARG_LEN_MAX];
    envval[0] = '\0';
    if(num_args==3){
        strcpy(envval, args[2]);
    }
    setenv(args[1], envval, 1);
    return 0;
}

int myunsetenv(int num_args, char **args){
    if(num_args != 2){
        fprintf(stderr,"shash: unsetenv: incorrect no. of arguments, 1-2 required\n");
        return 1;
    }
    unsetenv(args[1]);
    return 0;
}