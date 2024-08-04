#include "headers.h"

void RunInForeground(char *command ,char*username,char*systemname,char*initial_dir) {
    pid_t fg_pid = fork();
    if (fg_pid == 0) {
        struct timespec start, end;
        clock_gettime(CLOCK_MONOTONIC, &start);

        int result = system(command);

        clock_gettime(CLOCK_MONOTONIC, &end);
        double elapsed_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            char *relative_path = get_relative_path(cwd, initial_dir);
            if (strcmp(relative_path, "~") == 0) {
                printf("<%s@%s:%s> ", username, systemname, "~");
            } 
            else if(strcmp(relative_path,cwd)==0)
            {
                printf("<%s@%s:%s> ", username, systemname, relative_path); 
            }
            else
            {
                printf("<%s@%s:~%s> ", username, systemname, relative_path);
            }

            free(relative_path);
        } else {
            perror("getcwd() error");
            return ;
        }
        if(strstr(command,"sleep")!=NULL){
            printf("sleep : %.0fs>\n", elapsed_time);
        }
         if(strstr(command,"echo")!=NULL){
            printf("echo : %.0fs>\n", elapsed_time);
        }
         if(strstr(command,"vim")!=NULL){
            printf("vim : %.0fs>\n", elapsed_time);
        }

         if(strstr(command,"pwd")!=NULL){
            printf("pwd : %.0fs>\n", elapsed_time);
        }



        if (result == -1) {
            printf("ERROR: '%s' is not a valid command\n", command);
        }

        exit(0);
    } else {
        int status;
        waitpid(fg_pid, &status, 0);
    }
}

void RunInBackground(const char *command, BackgroundProcess bg_processes[], int *num_bg_processes) {
    // Fork a new process
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return;
    } else if (pid == 0) {
        // Child process
        // Execute the command using execlp
        execlp("sh", "sh", "-c", command, (char *)NULL);
        perror("execlp");
        exit(1);
    } else {
        // Parent process
        if (*num_bg_processes < MAX_BG_PROCESSES) {
            // Print the PID of the newly created background process
            printf("Background process %d started.\n", pid);

            // Add the background process to the array
            bg_processes[*num_bg_processes].pid = pid;
            strcpy(bg_processes[*num_bg_processes].command, command);
            bg_processes[*num_bg_processes].state=1;
            (*num_bg_processes)++;
        } else {
            printf("Maximum number of background processes reached.\n");
        }
    }
}
