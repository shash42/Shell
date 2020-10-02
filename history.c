#include "history.h"

//Get the history from the hidden file - oldest to newest
void getHistory(){
    char *path = (char *)malloc(PATH_MAX); path[0]='\0';
    strcpy(path, home);
    strcat(path, "/.history");
    FILE* temp = fopen(path, "a"); //creates file if doesnt exist
    fclose(temp);
    FILE* inp_file = fopen(path, "r");

    num_hist = 0;
    char *ret = (char *)malloc(LINE_LEN_MAX);
    size_t maxsz = sizeof(ret);
    //read front to back till EOF or HIST_LIM
    while(getline(&ret, &maxsz, inp_file)!=-1) {  
        //fprintf(stderr, "%d\n", num_hist); 
        strcpy(hist_list[num_hist], ret);      
        num_hist++;
        if(num_hist >= HIST_LIM) break;
    }

    fclose(inp_file);
    free(ret);  free(path);
}

//Write whole history, O(N)
void writeHistory(){
    char *path = (char *)malloc(PATH_MAX); path[0]='\0';
    strcpy(path, home);
    strcat(path, "/.history");
    
    FILE* out_file = fopen(path, "w");
    for(int i = 0; i < num_hist; i++){
        fprintf(out_file, "%s", hist_list[i]);
    }

    fclose(out_file);
    free(path);
}

//add line to history
void logHistory(char *toadd){
    getHistory();
    //fprintf(stderr, "%s", toadd);
    //If is at limit, remove first (oldest) command and shift all one step back.
    if(num_hist == HIST_LIM){
        for(int i = 0; i < num_hist - 1; i++){
            strcpy(hist_list[i], hist_list[i+1]);
        }
        num_hist--;
    }
    strcpy(hist_list[num_hist++], toadd);

    writeHistory();
}

//implementation of history command
int history(int num_args, char **args){
    getHistory();
    int req = HIST_DEF;
    if(num_args == 2){ //no. of commands required
        int asked = atoi(args[1]);
        req = asked;
    }
    else if(num_args > 2){
        fprintf(stderr, "shash: history: too many arguments");
        return 1;
    }
    if(req > num_hist) req = num_hist; //if more commands required than available, only show available
    //fprintf(stderr, "here%d", req);
    for(int i = num_hist - req; i < num_hist; i++){
        printf("%s", hist_list[i]);
    }
    return 0;
}