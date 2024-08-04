#include"headers.h"


void changeDirectoryAndUpdateHistory(const char* new_dir, const char* initial_dir,
                                     char abs_path_history[][1024], int*  history_count) {
    if (chdir(new_dir) != 0) {
        perror("chdir() error");
    } else {          
        char new_cwd[1024];
        if (getcwd(new_cwd, sizeof(new_cwd)) != NULL) {
            // printf("new_cwd : %s\n",new_cwd);
            // printf("history_count:%d\n",*history_count);
            strcpy(abs_path_history[*(history_count)],new_cwd);
            *(history_count)= *(history_count)+1;
            // printf("history_count:%d\n",*history_count);
            // printf("new_cwd : %s\n",new_cwd);
            char* relative_path = get_relative_path(new_cwd, initial_dir);
            char absolute_path[1024];
            getcwd(absolute_path, sizeof(absolute_path));
            printf("%s\n", absolute_path);
            free(relative_path);
        } else {
            perror("getcwd() error");
        }
    }
}

void handleWarpCommand(const char* input, const char* initial_dir,char*cwd,int* history_count,char abs_path_history[][1024]) {
    if(strcmp(input,".")==0)
    {
        chdir(cwd);
         //printf("history_countttt:%d\n",*history_count);
        changeDirectoryAndUpdateHistory(cwd, initial_dir, abs_path_history, history_count);
       // printf("history_counttt:%d\n",history_count);
    }
    else if(strcmp(input,"~")==0)
    {
        chdir(initial_dir);
        changeDirectoryAndUpdateHistory(initial_dir, initial_dir, abs_path_history, history_count);
    }
    else if(strcmp(input,"-")==0)
    {
        if (*history_count >= 2) {
            chdir(abs_path_history[*history_count - 2]);
            if (getcwd(abs_path_history[*history_count], sizeof(abs_path_history[*history_count])) != NULL) {
                *(history_count)= *(history_count)+1;
            } else {
                perror("getcwd() error");
            }
            char new_cwd[1024];
            if (getcwd(new_cwd, sizeof(new_cwd)) != NULL) {
                char* relative_path = get_relative_path(new_cwd, initial_dir);
                char absolute_path[1024];
                getcwd(absolute_path, sizeof(absolute_path));
                printf("%s\n", absolute_path);
                free(relative_path);
            } else {
                perror("getcwd() error");
            }
        } else {
            printf("Not enough history.\n");
        }
    }
     else if (strcmp(input, "..") == 0) {
        chdir("..");
        //printf("reached\n");
        char cwd[1024];
        getcwd(cwd,sizeof(cwd));
        //printf("cwd:%s--------\n",cwd);
        //printf("history count:%d\n",history_count);
        strcpy(abs_path_history[*history_count],cwd);
       // printf("cwd:%s\n",abs_path_history[history_count-1]);
       *(history_count)= *(history_count)+1;
        char new_cwd[1024];
        if (getcwd(new_cwd, sizeof(new_cwd)) != NULL) {
            char* relative_path = get_relative_path(new_cwd, initial_dir);
            char absolute_path[1024];
            getcwd(absolute_path, sizeof(absolute_path));
            printf("%s\n", absolute_path);
            free(relative_path);
        } else {
            perror("getcwd() error");
        }
    } 
    else
    {
        char dir_name[1024];
        strcpy(dir_name,input);
            
            // Change the directory
            if (chdir(dir_name) != 0) {
                perror("chdir() error");
            } else {
                // Store the absolute path in the history array
                if (getcwd(abs_path_history[*history_count], sizeof(abs_path_history[*history_count])) != NULL) {
                    *history_count=*history_count+1;
                } else {
                    perror("getcwd() error");
                }
                
                // Print the new relative path after changing directory
                char new_cwd[1024];
                if (getcwd(new_cwd, sizeof(new_cwd)) != NULL) {
                    char *relative_path = get_relative_path(new_cwd, initial_dir);
                  char absolute_path[1024];
                     getcwd(absolute_path, sizeof(absolute_path));
                    printf("%s\n",absolute_path);
                    free(relative_path);
                } else {
                    perror("getcwd() error");
                }
            }

       
        }
    }    
