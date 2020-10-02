#include "ls.h"

int hide_filter(const struct dirent *entry)
{
    if(entry->d_name[0]=='.') return 0;
    return 1;
}

//Printing -l output
void lslong(char *actpath, char *actname){
    //Fix path for stat
    char *path = (char *)malloc(sizeof(char) * PATH_MAX);
    path[0] = '\0';
    strcpy(path, actpath);
    strcat(path, "/\0");
    strcat(path, actname);

    struct stat st;
    if(stat(path, &st) < 0){
        perror("shash: ls");
        return;
    }

    int perms[9] = {S_IRUSR, S_IWUSR, S_IXUSR, S_IRGRP, S_IWGRP,
                    S_IXGRP, S_IROTH, S_IWOTH, S_IXOTH};
    char perm_print[11], permchars[3] = {'r', 'w', 'x'};

    //First column
    if(S_ISDIR(st.st_mode)) perm_print[0] = 'd';
    else perm_print[0] = '-';
    for(int i = 0; i < 9; i++){
        if(st.st_mode & perms[i]){
            perm_print[i+1] = permchars[i%3];
        }
        else perm_print[i+1] = '-';
    }
    perm_print[10] = '\0';

    //Other columns
    char *grp = (getgrgid(st.st_gid))->gr_name;
    char *username =  (getpwuid(st.st_uid))->pw_name;
    char *modtime = (char *)malloc(20); 
    strftime(modtime, 20, "%b %e %H:%M", localtime(&st.st_mtime));

    //Print
    printf("%s %ld %s %s ", perm_print, st.st_nlink, grp, username);
    printf("%ld %s %s\n", st.st_size, modtime, actname);
    
    //Free
    free(modtime);
    free(path);
}

//do ls for a particular folder
void execls(char *dirpath, int aflag, int lflag){
    char *actpath = getRootDir(dirpath);
    struct dirent** scandir_ret;
    int n;

    //handle aflag
    if(aflag==0) n = scandir(actpath, &scandir_ret, hide_filter, alphasort);
    else n = scandir(actpath, &scandir_ret, NULL, alphasort);
    if (n == -1) {
        perror("shash: scandir");
        return;
    }

    //handle l flag and print
    for(int i = 0; i < n; i++)
    {
        if(lflag==0) printf("%s\n", scandir_ret[i]->d_name);
        else lslong(actpath, scandir_ret[i]->d_name);
        free(scandir_ret[i]);
    }
    free(scandir_ret);
}

//ls command implementation 
void ls(int num_args, char **args){
    extern char *optarg;
	extern int optind;
    optind = 1;
	int c; 
	int aflag=0, lflag=0;

    //Check if there are any flags
	while ((c = getopt(num_args, args, "al")) != -1) {
		switch (c) {
		case 'a':
			aflag = 1;
			break;
		case 'l':
			lflag = 1;
			break;
		case '?':
			break;
		}
    }

    //Check if there are any non-flag options (like paths)
    int num_dir = num_args - optind;
    if(num_dir==0){
        execls(cwd, aflag, lflag); //if no flag, ls on cwd.
        return;
    }
    for(int i = optind; i < num_args; i++){ //else ls on every path provided
        if(i > optind){
            printf("\n"); //print blank line after each indiv. path's ls output
        }
        if(num_dir > 1){
            printf("%s:\n", args[i]);
        }
        execls(args[i], aflag, lflag);
    }
}