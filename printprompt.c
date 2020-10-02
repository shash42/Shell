#include "printprompt.h"

//Get the user-name
char* getUser(){

    struct passwd *pw;
    pw = getpwuid(getuid());
    //TODO: Error Handling
    return pw->pw_name;
}

//Get the host name
char *getHost(){
    struct utsname buf;
    uname(&buf);
    //TODO: Error Handling
    char *ret = (char *) malloc (sizeof (char) * strlen(buf.nodename));
    ret[0] = '\0';
    strcpy(ret, buf.nodename);
    return ret;
}

//Print the prompt in format required
void printPrompt(){
    char user_name[LOGIN_NAME_MAX + 2], sys_name[HOST_NAME_MAX + 2], dir_name[PATH_MAX + 2];
    user_name[0] = '\0'; sys_name[0] = '\0';
    strcpy(user_name, strdup(getUser()));
    strcpy(sys_name, strdup(getHost()));
    char *shell_dir = getShellDir(cwd);
    strcpy(dir_name, shell_dir);
    free(shell_dir);
    printf("%s@%s:%s$ ", user_name, sys_name, dir_name);
}