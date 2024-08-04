#include "headers.h"
#include "warp.h"
#include "peek.h"
#include "seek.h"
#include "proclore.h"

void execute_pastevents(char command_history[][1024],int*command_count)
{
    int idx=15;
            if(*command_count<15)
            {
                idx=*command_count;
            }
            for (int i = 0; i < idx; i++) {
                printf("%s\n", command_history[i]);
            }
}
void execute_pastevents_purge(char command_history[][1024],int*command_count)
{
    *command_count=0;
            char*idx ="\0";
            for (int i = 0; i < 15; i++) {
               strcpy(command_history[i],idx);
            }
}
void execute_the_command(char*input,char initial_dir[1024],char*cwd,char abs_path_history[][1024],int*history_count)
{        
        char*token=strtok(input, " \t");
        //printf("Token:%s\n",token);
         int valid=0;
        if(strcmp(token,"warp")==0)
        {
            valid=1;
            token = strtok(NULL," \t");
            // printf("token:%s <---\n",token);
            if(token==NULL)
            {
                chdir(initial_dir);
                printf("%s\n",initial_dir);
            }
            while(token!=NULL)
            {
                //printf("token:%s &&&&\n",token);
                handleWarpCommand(token,initial_dir,cwd,history_count,abs_path_history);
               // printf("history_count:%d\n",history_count);
                token= strtok(NULL, "\t");
            }    
        }
        else if(strcmp(token,"peek")==0)
        {
            valid=2;
            token = strtok(NULL, " \t");
            //printf("TOKEN:%s\n",token);
            if(token!=NULL)
            {
                handlePeekCommand(token,initial_dir,cwd ,history_count,abs_path_history);
            }
           // handlePeekCommand(input,initial_dir,abs_path_history,history_count);
        }
        else if(strcmp(token,"proclore")==0)
        { 
                // printf("reached\n");
                //  printf("str_num:-%s-\n",token)
                valid=3;
                 token = strtok(NULL, " \t");
               //  printf("str_num:-%s-\n",token);
                // //snprintf(str_num, sizeof(str_num), "%d", num);
                // int flag=0;
                 if(token==NULL){
                   // flag=1;
                    printf("pid: %d\n", getpid());
                    char pid[20];
                    sprintf(pid,"%d",getpid());
                    print_process_info(pid);
                 }
                 else{
                    //int num=atoi(point);
                    //printf("pid: %d\n", num);
                    //flag=2;
                   // pid_t num = atoi(str_num);
                    printf("pid: %s\n", token);
                    print_process_info(token);
                 }
                // printf("FLAG:%d\n",flag);
        }
        else if(strcmp(token,"seek")==0)
        {
            valid=8;
            token = strtok(NULL, " \t");
                int file=0;
                int dir=0;
                int eflag=0;
                while(token!=NULL && token[0]=='-'){
                     if(token[1]=='f'){
                        file=1;
                     }
                     else if(token[1]=='d'){
                        dir=1;
                     }
                     else if(token[1]=='e'){
                           eflag=1;
                     }
                     token=strtok(NULL, " \t");
                }
                char search[256]; 
                strcpy(search,token);
                char check_for_seeking[256];
                char* next_token;
                token=strtok(NULL, " \t");
                if(token==NULL){
                    next_token = malloc(1000); 
                if (next_token != NULL) {
                    strcpy(next_token, ".");
                } 
                else {
                    perror("malloc");
                }
                }
                if(token!=NULL){
                strcpy(next_token,token);
                }
               if (strcmp(next_token,".")==0 || strcmp(next_token,";")==0) {
               
                if(getcwd(check_for_seeking,sizeof(check_for_seeking))!=NULL){
                        //printf("%s\n",check_for_seeking);
                        strcpy(next_token,check_for_seeking);
                        //printf("%s\n",token);
                    }
                 }
                 if(strcmp(next_token,"~")==0){
                    strcpy(next_token,initial_dir);
                 }
                 if(next_token[0]=='.' && next_token[1]!='\0'){
                    if(getcwd(check_for_seeking,sizeof(check_for_seeking))!=NULL){
                        strcat(check_for_seeking,next_token+1);
                        strcpy(next_token,check_for_seeking);
                    }
                 }
                 char directory[256];
                 strcpy(directory,next_token);
                seekCommand(search, directory, file,dir,eflag );
        }
        if(valid==0)
    {
       printf("%s is not a valid command\n",input);
    }
}

void execute_pastevents_execute(int num,char command_history[][1024],int*command_count,char initial_dir[1024],char*cwd,char*username,char*systemname,char abs_path_history[][1024],int*history_count)
{
    char command_to_execute[1024];
    //printf("command_to_execute :%s\n",command_history[*(command_count)-num]);
    strcpy(command_to_execute, command_history[*(command_count)-num]);
    //printf("command_to_execute :%s\n",command_to_execute);
    execute_the_command(strdup(command_to_execute),initial_dir,cwd,abs_path_history,history_count);
    if(num!=1){
        if (*command_count < 15) 
        {
           // printf("command_to_execute :%s\n",command_to_execute);
            strcpy(command_history[*command_count], command_to_execute);
           //  printf("command_history[*command_count]:%s\n",command_history[*command_count]);
        } 
        else 
        {
            for (int i = 0; i < 14; i++) 
            {
                strcpy(command_history[i], command_history[i + 1]);
            }
            strcpy(command_history[14], command_to_execute);
        }
        *(command_count)=*(command_count)+1;
    }        

}
