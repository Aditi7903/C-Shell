#include "headers.h"

void listProcesses(BackgroundProcess*bg_processes, int* num_bg) {
    printf("Command Name : pid - state\n");
    char st[20];
    for (int i = 0; i < *num_bg; i++) {
        //printf("state:%d\n",bg_processes[i].state);
        if(bg_processes[i].state==1)
        {
           // printf(here);
            strcpy(st,"RUNNING");
        }
        else if(bg_processes[i].state==0)
        {
            strcpy(st,"STOPPED");   
        }
        else if(bg_processes[i].state==-1)
        {
            strcpy(st,"EXITED");
        }
        printf("%s : %d - %s\n", bg_processes[i].command, bg_processes[i].pid,st);
        strcpy(st,"\0");
    }
}
