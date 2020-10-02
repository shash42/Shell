#include "nightswatch.h"

//implements interrupt flag
void interrupt()
{
    char *path = "/proc/interrupts";
    char *line = readFileLine(path, 3); //openable because checked before
    int i = 0;
    while(line[i]!='I') { //heuristic to parse info
        i++;
    }
    line[i-1] = '\0';
    printf("%s\n", line);
    free(line);
}

//handles newborn flag
void newborn(){
    char *path = "/proc/loadavg";
    FILE *file_int = fopen(path, "r");
    if(file_int==NULL){
        perror("shash: can't open loadavg");
        return;
    }
    fclose(file_int);
    char *line = readFileLine(path, 1);
    char *temp= strtok(line, " \n");
    for(int i = 0; i < 4; i++){
        temp = strtok(NULL, " \n");
    } //heuristic to parse reqd info.
    printf("%s\n", temp);
    free(line);
}

void nightswatch(int num_args, char **args){
    if(num_args != 4){
        printf("Usage: nightswatch -n [seconds] [\"newborn\" | \"interrupt\"]\n");
        return;
    }
    else if(strcmp("-n", args[1])!=0){
        printf("Invalid arguments: Usage: nightswatch -n [seconds] [\"newborn\" | \"interrupt\"]\n");
        return;
    }
    int num_sec = atoi(args[2]);
    int i_or_n;
    if(strcmp("interrupt", args[3])==0){
        i_or_n = 0;
    }
    else if(strcmp("newborn", args[3])==0){
        i_or_n = 1;
    }
    else{
        printf("Invalid arguments: Usage: nightswatch -n [seconds] [\"newborn\" | \"interrupt\"]\n");
        return;
    }

    int ms = 0, msgap = num_sec*1000;
    clock_t lasttime = clock();

    if(i_or_n==0){
        //check if interrupts is readable.
        char *path = "/proc/interrupts";
        FILE *file_int = fopen(path, "r");
        if(file_int==NULL){
            perror("shash: can't open interrupts");
            return;
        }

        char *line = readFileLine(path, 1);
        printf("%s", line);
        fclose(file_int);
        free(line);
    }

    while(1){
        ms = (clock() - lasttime) * 1000 / CLOCKS_PER_SEC; //compute time since last check

        //make I/O non-blocking
        fd_set readfds;
        struct timeval tv; tv.tv_sec = 0; tv.tv_usec = 0;
        FD_ZERO(&readfds);
        FD_SET(fileno(stdin), &readfds);
        if(select(1, &readfds, NULL, NULL, &tv))
        {
            char key = fgetc(stdin);    fgetc(stdin);
            if(key=='q') break;
        }
        
        //if more time has passed than reqd gap, print!
        if(ms >= msgap){
            lasttime = clock(); //reset lasttime
            if(i_or_n){
                newborn();
            }
            else{
                interrupt();
            }
        }
    }
}