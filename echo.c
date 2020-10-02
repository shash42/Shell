#include "echo.h"

int echo(int num_args, char **args){
    char *output = (char *)malloc(ECHO_LEN_MAX);
    char *addspace = " ";
    output[0] = '\0';
    for(int i = 1; i < num_args; i++){
        if(i>1){
            //add one space between each token (as all whitespace was initially ignored)
            strcat(output, addspace);
        }
        strcat(output, args[i]);
    }
    printf("%s\n", output);
    //free(output);
    return 0;
}