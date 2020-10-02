#include "nightswatch.h"

//implements interrupt flag
int interrupt()
{
    char *path = "/proc/interrupts";
    char *line = readFileLine(path, 3); //openable because checked before
    if(!line || strlen(line)<0){
        perror("shash: interrupt");
        return 1;
    }
    int i = 0;
    while(line[i]!='I') { //heuristic to parse info
        i++;
        if(i>500) break;
    }
    if(i>500){
        fprintf(stderr, "shash: interrupt: incompatible format of /proc/interrupts\n");
        return 1;
    }
    line[i-1] = '\0';
    printf("%s\n", line);
    free(line);
    return 0;
}

//handles newborn flag
int newborn(){
    char *path = "/proc/loadavg";
    FILE *file_int = fopen(path, "r");
    if(file_int==NULL){
        perror("shash: newborn: can't open loadavg");
        return 1;
    }
    fclose(file_int);

    char *line = readFileLine(path, 1);
    char *temp= strtok(line, " \n");
    for(int i = 0; i < 4; i++){
        temp = strtok(NULL, " \n");
    } //heuristic to parse reqd info.
    printf("%s\n", temp);
    free(line);
    return 0;
}

int nightswatch(int num_args, char **args){
    if(num_args != 4){
        printf("Usage: nightswatch -n [seconds] [\"newborn\" | \"interrupt\"]\n");
        return 1;
    }
    else if(strcmp("-n", args[1])!=0){
        printf("Invalid arguments: Usage: nightswatch -n [seconds] [\"newborn\" | \"interrupt\"]\n");
        return 1;
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
        return 1;
    }

    int ms = 0, msgap = num_sec*1000;
    clock_t lasttime = clock();

    if(i_or_n==0){
        //check if interrupts is readable.
        char *path = "/proc/interrupts";
        FILE *file_int = fopen(path, "r");
        if(file_int==NULL){
            perror("shash: can't open interrupts");
            return 1;
        }

        char *line = readFileLine(path, 1);
        printf("%s", line);
        fclose(file_int);
        free(line);
    }

    int ret = 0;
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
                ret = newborn();
                if(ret > 0){
                    return ret;
                }
            }
            else{
                ret = interrupt();
                if(ret > 0){
                    return ret;
                }
            }
        }
    }
    return 0;
}