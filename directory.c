#include "directory.h"

//read line line_num from file at path
char *readFileLine(char *path, int line_num){
    char *ret = (char *)malloc(PROC_LINE_MAX);
    FILE* inp_file = fopen(path, "r");
    int i = 1;
    size_t maxsz = sizeof(ret);
    while(getline(&ret, &maxsz, inp_file)!=-1) {
        if(i==line_num){
            break;
        }
        i++;
    }

    fclose(inp_file);
    return ret;
}

//Check if home directory is prefix of current path
int isPrefix(int home_len, int cwd_len, char *curr){
    if(home_len > cwd_len) return 0;
    for(int i = 0; i < home_len; i++){
        if(curr[i]!=home[i]) return 0;
    }
    return 1;
}

//Get shell directory (truncate home prefix to ~)
char *getShellDir(char *curr){
    int home_len = strlen(home), cwd_len = strlen(curr);
    int isP = isPrefix(home_len, cwd_len, curr);
    char *shell_dir = (char *) malloc(PATH_MAX + 2);
    if(isP){
        shell_dir[0] = '~';
        for(int i = home_len; i < cwd_len; i++){
            shell_dir[i - home_len + 1] = curr[i];
            printf("%c", curr[i]);
        }
        shell_dir[cwd_len - home_len + 1] = '\0';
    }
    else {
        strcpy(shell_dir, curr);
    }
    return shell_dir;
}

//Get full path from ~ truncated path
char *getRootDir(char *inppath){
    char *reqd_path = (char *)(malloc(PATH_MAX));
    if(inppath[0]!='~'){
        strcpy(reqd_path, inppath);
    }
    else{
        int len_path = strlen(inppath);
        char *path = (char *)(malloc(PATH_MAX));

        for(int i = 1; i < len_path; i++){
            path[i - 1] = inppath[i];
        }
        path[len_path - 1] = '\0';
        strcpy(reqd_path, home); strcat(reqd_path, path);
        free(path);
    }
    return reqd_path;
}

//Change directory
int cd(int num_args, char **args){
    if(num_args > 2){
        printf("shash: cd: too many arguments\n");
        return 1;
    }

    else if(num_args==1){
        if(chdir(home) < 0){
            perror("shash: cd");
            return 1;
        }
        else{
            //incase needed before next command's tokenizatoin begins (eg: piping) 
            strcpy(lwd, cwd);
            getcwd(cwd, PATH_MAX + 2);
        }
    }
    
    else if(strcmp(args[1], "-")==0){
        char temp[PATH_MAX];
        strcpy(temp, cwd);
        if(strlen(lwd)==0){
            printf("%s\n", cwd);
        }
        else if(chdir(lwd) < 0){
            perror("shash: cd");
            return 1;
        }
        else{
            printf("%s\n", lwd);
            strcpy(lwd, cwd);
            getcwd(cwd, PATH_MAX + 2);
        }
    }

    else{ //move to root directory
        char *reqd_path = getRootDir(args[1]);
        if(chdir(reqd_path) < 0){
            perror("shash: cd");
            return 1;
        }
        else{
            strcpy(lwd, cwd);
            getcwd(cwd, PATH_MAX + 2);
        }
        free(reqd_path);
    }
    return 0;
}

//Print present working directory
int pwd(int num_args, char **args){
    printf("%s\n", cwd);
    return 0;
}