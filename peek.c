#include "headers.h"

char *entry_permissions(mode_t mode) {
    static char perms[11];
    strcpy(perms, "----------");
    if (S_ISDIR(mode)) perms[0] = 'd';
    if (mode & S_IRUSR) perms[1] = 'r';
    if (mode & S_IWUSR) perms[2] = 'w';
    if (mode & S_IXUSR) perms[3] = 'x';
    if (mode & S_IRGRP) perms[4] = 'r';
    if (mode & S_IWGRP) perms[5] = 'w';
    if (mode & S_IXGRP) perms[6] = 'x';
    if (mode & S_IROTH) perms[7] = 'r';
    if (mode & S_IWOTH) perms[8] = 'w';
    if (mode & S_IXOTH) perms[9] = 'x';
    return perms;
}


void print_file_details(struct dirent *entry, char *abs_path) {
    struct stat st;
    if (stat(abs_path, &st) == 0) {
        if (S_ISDIR(st.st_mode)) {
            printf("\033[34m%s\033[0m\n", entry->d_name);  // Blue for directories
        } else if (st.st_mode & S_IXUSR) {
            printf("\033[32m%s\033[0m\n", entry->d_name);  // Green for executables
        } else {
            printf("%s\n", entry->d_name);  // Default color for regular files
        }

        // Print additional file attributes if needed
        if (st.st_mode & S_ISUID) printf("\033[41m");
        if (st.st_mode & S_ISGID) printf("\033[42m");
        if (st.st_mode & S_ISVTX) printf("\033[46m");
        if (st.st_mode & S_IRUSR) printf("\033[30;41m");
        if (st.st_mode & S_IWUSR) printf("\033[30;42m");
        if (st.st_mode & S_IXUSR) printf("\033[30;43m");
        printf("\033[0m ");
    }
}

void get_the_list_of_files(const char *full_path, bool show_details, bool show_hidden) {
    struct dirent **namelist;
    int num_entries;

    num_entries = scandir(full_path, &namelist, NULL, alphasort);
    if (num_entries == -1) {
        perror("scandir() error");
        return;
    }

    for (int i = 0; i < num_entries; i++) {
        if (!show_hidden && namelist[i]->d_name[0] == '.') {
            free(namelist[i]);
            continue;
        }

        char abs_path[2048];
        snprintf(abs_path, sizeof(abs_path), "%s/%s", full_path, namelist[i]->d_name);
        struct stat st;

        if (show_details && stat(abs_path, &st) == 0) {
            printf("%s ", entry_permissions(st.st_mode));
            printf("%2lu ", st.st_nlink);
            printf("%-8s %-8s ", getpwuid(st.st_uid)->pw_name, getgrgid(st.st_gid)->gr_name);
            printf("%8lld ", (long long)st.st_size);
            printf("%.12s ", ctime(&st.st_mtime) + 4);
        }

        // Print file name with color
        if (stat(abs_path, &st) == 0) {
            if (S_ISDIR(st.st_mode)) {
                printf("\033[34m%s\033[0m\n", namelist[i]->d_name);  // Blue for directories
            } else if (st.st_mode & S_IXUSR) {
                printf("\033[32m%s\033[0m\n", namelist[i]->d_name);  // Green for executables
            } else {
                printf("%s\n", namelist[i]->d_name);  // Default color for regular files
            }
        }

        free(namelist[i]);
    }
    free(namelist);
}
void handlePeekCommand(char* input, const char* initial_dir,const char* cwd, int* history_count,char abs_path_history[][1024]) {
    bool show_hidden = false;
    bool show_details = false;
    int catch = 0;
    //printf("next token:%s\n",input);
    //int flag=0;
    //printf("nexxxxxxxt tokennnnnnnn:%c\n",input[0]);
    if(input[0]=='-' && (input[1]=='\0' || input[1]=='\n'))
    {
        strcpy(input,"-");
      //  printf("next token:%s\n",input);
    }
    else
    {
    while (input != NULL && input[0] == '-' && (input[1] != '\0'|| input[1] !='\n')) {
        //printf("nexxxxxxxtttttt tokennnnnnnn:%c\n",input[0]);
        for (int i = 1; input[i] != '\0'; i++) {
            if (input[i] == 'a') {
                show_hidden = true;
            }
            if (input[i] == 'l') {
                show_details = true;
            }
        }
          //printf("nexxxxxt token:%s\n",input);
        input = strtok(NULL, "\t");
        //printf("next tokennn:%s\n",input);
        //flag=1;
         if(input==NULL)
        {
            //printf("heeeere\n");
            break;

        }
        if(input[0]=='/')
        {
            break;
        }
        if(input[0]=='-'&& (input[1]=='\0' || input[1]=='\n'))
        {
           // printf("eere\n");
            break;
        }
       
    }
    }
    
    //printf("nexxxxxxxt tokennnnnnnn:%s\n",input);
    // if(flag==0)
    // {
    //     input[0]=='-';
    //     printf("nexxxxxxxt tokennnnnnnn:%c\n",input[0]);
    // }
    //printf("next tokennnnnnnn:%s\n",input);
        char dir[1024];
    if(input==NULL)
    {
         strcpy(dir,cwd);
        //printf("here\n");
    }
    else
    {
    if(input[0]=='-')
    {
       // printf("reached--\n");
        if(*(history_count)>=2)
        {
            strcpy(dir,abs_path_history[*(history_count)-2]);
        }
        else
        {
            printf("NOT ENOUGH HISTORY!!!\n");
        }
      
    }
    else if(strcmp(input,".")==0 )
    {
        strcpy(dir,cwd);
       // printf("dir:%s\n",dir);
    }
    else if(strcmp(input,"~")==0)
    {
        strcpy(dir,initial_dir);
    }
    else if(strcmp(input,"..")==0)
    {
        char get_it[1024];
        if(getcwd(get_it,sizeof(get_it))!=NULL)
        {
            char*prev_dir=strrchr(get_it,'/');
            get_it[strlen(get_it)-strlen(prev_dir)]='\0';
            strcpy(dir,get_it);
        }
    }
    else if(strcmp(input,"~")==0)
    {
        char get_it[1024];
        strcpy(get_it, initial_dir);
        strcat(get_it, input+1);
        strcpy(dir,get_it);
    }
    else if(input[0]=='/')
    {
        //printf("reacehd cicbv\n");
        char temp[1024];
        char directory[1024];
        //printf("reacehd cicbv\n");
        strcpy(directory,input);
        //printf("initial_sir:%s  and directoty: %s\n",initial_dir,directory);
        if(strstr(directory,initial_dir)!=NULL)
        {
            //printf("reacehd cicbv\n");
            strcpy(dir,directory);
        }
        else
        {
           // printf("read cicbv\n");
           if(strstr(initial_dir,directory)!=NULL)
           {
             strcpy(dir,directory);
           }
           else
           {
            strcpy(dir,initial_dir);
            strcat(dir,input);
           }

        }
        
    }
    }
    //printf("cwd:%s\n",dir);
    get_the_list_of_files(dir,show_details,show_hidden);
}