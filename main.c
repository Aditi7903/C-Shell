#include "headers.h"
#include "prompt.h"
#include "warp.h"
#include "peek.h"
#include "pastevents.h"
#include "system.h"
#include "proclore.h"
#include "seek.h"
#include "input.h"
#include "neonate.h"
#include "iMan.h"
#include "redirection.h"
#include "activities.h"
#include "ping.h"
#include "ctrl.h"

struct termios orig_termios;
bool is_foreground = false;
pid_t foreground_pid;
int main(int argc, char*argv[]) 
{    
    signal(SIGINT, sigint_handler);
    signal(SIGTSTP, sigtstp_handler);
    char initial_dir[1024];
    getcwd(initial_dir, sizeof(initial_dir));
    char*sys_home_dir=getenv("HOME");
    char *username = get_username();
    char *systemname = get_system_name();
    
    char abs_path_history[100][1024]; // Array to store absolute paths
    int history_count = 0; // Number of stored paths
    strcpy(abs_path_history[0],initial_dir);
    history_count++;
    char command_history[15][1024];  // 2D array to store command history
    int command_count = 0;
    int come_out=0;
    BackgroundProcess bg_processes[MAX_BG_PROCESSES];
    int num_bg_processes = 0;
    int pid,signalNumber;
    while (1) 
    {
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
            return 1;
        }
        
        char input[1024];
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break; 
        }
        pid_t bg_pids[20]; // Assuming max of 20 background processes
        int bg_count = 0;
   

        input[strcspn(input, "\n")] = '\0';


         if (feof(stdin)) {
            
            printf("Ctrl+D detected. Logging out...\n");
            
            // Clean up and exit the shell
            // ... (Perform any necessary cleanup, such as killing background processes)
            
            exit(0);
        }        


        for (int i = 0; i < num_bg_processes; i++) {
        int status;
        pid_t ended_pid = waitpid(bg_processes[i].pid, &status, WNOHANG);
        if (ended_pid > 0) {
            char str[128];
            strcpy(str, bg_processes[i].command);
            str[strlen(str) - 1] = '\0';
           // printf("STATE:%d\n",bg_processes[i].state);
             if( bg_processes[i].state==1){
            if (WIFEXITED(status)) {
                printf("Background process %d (%s) exited normally\n", ended_pid, str);
                bg_processes[i].state=0;
                
            } else {
                printf("Background process %d (%s) exited abnormally\n", ended_pid, str);
                bg_processes[i].state=0;
            }
             }
            // Remove the process from the array (shift elements)
           
            i--; // Re-check the same index, as the next element has moved here
        }
    }
      

        int valid=0;

        
        if (input[strlen(input) - 1] == '&') 
        {
            char check[256];
            strcpy(check,input);
            check[strlen(check)-1]='\0';
            RunInBackground(check, bg_processes, &num_bg_processes);
            valid=1;
        }
        else if (strstr(input,"&"))
        {
            int idx=0;
            char user_input[1024];
            strcpy(user_input,input);
            int check=0;
            while(idx<strlen(user_input)-1)
            {
                if(user_input[idx]=='&')
                {
                    check=1;
                }
                idx++;
            }
            idx=0;
            //printf("Input: %s\n",user_input);
            if(check==1){
            int get_pos=0;
            while(idx<strlen(user_input)-1)
            {
                if(user_input[idx]=='&')
                {
                    get_pos=idx;
                }
                idx++;
            }

        char user_input1[1024];
        idx=get_pos+1;
        int k=0;
        while(idx<strlen(user_input))
        {
            user_input1[k++]=user_input[idx];
            idx++;
        }
        user_input1[k]='\0';
        user_input[get_pos]='\0';
        RunInBackground(user_input, bg_processes, &num_bg_processes);
        runInForeground(user_input1);
        valid=8573;
        }

        }
        else if (strstr(input,"iMan")!=NULL)
        {
            char *command_name = input + 5; 
            resolveAndOpenSocket("man.he.net", 80, command_name);
            valid=777;    
        }
        else if(strstr(input,"<")!=NULL || strstr(input,">")!=NULL || strstr(input,">>")!=NULL)
        {
            Command cmd;
            cmd.cmd[0] = '\0';
            cmd.infile[0] = '\0';
            cmd.outfile[0] = '\0';
            cmd.append = false;
            char*token = strtok(input, " \n");
            while (token != NULL) {
            if (strcmp(token, "<") == 0) {
                token = strtok(NULL, " \n");
                strcpy(cmd.infile, token);
            } else if (strcmp(token, ">") == 0) {
                token = strtok(NULL, " \n");
                strcpy(cmd.outfile, token);
            } else if (strcmp(token, ">>") == 0) {
                token = strtok(NULL, " \n");
                strcpy(cmd.outfile, token);
                cmd.append = true;
            } else {
                strcat(cmd.cmd, token);
                strcat(cmd.cmd, " ");
            }
            token = strtok(NULL, " \n");
        }

        // Execute the command with I/O redirection
        ExecuteCommand(&cmd);   
        valid=835;
        }
        else if(strstr(input,"|")!=NULL)
        {
            valid=738;
            char *pipe_token = strchr(input, '|');
            if (pipe_token != NULL) {
            // Split the input into two commands
            *pipe_token = '\0';

            ShellCommand command1, command2;
            strcpy(command1.command, input);
            strcpy(command2.command, pipe_token + 1);

            int pipe_fds[2];
            if (pipe(pipe_fds) == -1) {
                perror("pipe");
                exit(EXIT_FAILURE);
            }

            // Execute the first command with stdout redirected to the write end of the pipe
            execute_shell_command(&command1, STDIN_FILENO, pipe_fds[1]);
            close(pipe_fds[1]);

            // Execute the second command with stdin redirected from the read end of the pipe
            execute_shell_command(&command2, pipe_fds[0], STDOUT_FILENO);
            close(pipe_fds[0]);
        }
        }
        else if (strstr(input,"cat")!=NULL)
        {
            runInForeground(input);
            valid=83;
        }
        else if (strstr(input,"echo")!=NULL ||strstr(input,"pwd")!=NULL || strstr(input,"vim")!=NULL  ||  strstr(input,"sleep")!=NULL) 
        {
            RunInForeground(input,username,systemname,initial_dir);
           
            valid=2;
        } 
        else {
             if (strncmp(input, "neonate -n", 10) == 0) {
                int time_arg;
                if (sscanf(input + 11, "%d", &time_arg) == 1) {
                    neonate(time_arg);
                } 
                else {
                    printf("Invalid usage of neonate. Example usage: neonate -n [time_arg]\n");
                }
                valid=594;
                continue; // Skip the rest of the loop
            }
           else if(strcmp(input,"activities")==0)
            {
            //  printf("STATE:%d\n",bg_processes[num_bg_processes-1].state);
                listProcesses(bg_processes,&num_bg_processes);
                valid=2611;
            }
            else  if (sscanf(input, "ping %d %d", &pid, &signalNumber) == 2) 
            {
                pingProcess(pid, signalNumber,bg_processes,&num_bg_processes);
                valid=440;
            }
            else if(strstr(input,"|")==NULL)
            {
            valid=994;
            
        char*Token=strtok(strdup(input), "\t;\t");
        //printf("TOKEN:%s\n",Token);
         //printf("INPUT: %s\n",input);
        Token=strtok(strdup(input),"\t;\t");
        char *trimmedToken = Token;
        while (*trimmedToken == ' ' || *trimmedToken == '\t') {
        trimmedToken++;
        }
      //  printf("TOKAN: %s\n",trimmedToken);
        Token=trimmedToken;
        
        while(Token!=NULL){
            char CWD[1024];
            getcwd(CWD,sizeof(CWD));
        int index=strlen(Token)-1;
        int len=index;
        // printf("len:%d\n",strlen(Token));
        // printf("len:%d\n",len);
        while(Token[index]==' ')
        {
            len--;
            index--;
        }
        Token[len+1]='\0';
        // printf("TOkENNNNNNNN :-%s-\n",Token);
        // printf("len:%d\n",strlen(Token));
        // printf("len:%d\n",len);       
        int flag=0;
       // Token[strlen(Token)]=' ';
        
            for (int i = 0; i < 14; i++) {
                //  printf("command_history[%d]:-%s-\n",i,command_history[i]);
                //   printf("TOka _tokaa :-%s-\n",Token);
                if(strcmp(command_history[i], Token)==0)
                {
                    flag=1;
                    //printf("imposter found , begen ka luad agand marwa le bsdk\n");
                }
            }
        // printf("TOKEN:%s\n",Token);
        // printf("\nFLAG:%d\n",flag);
        if(strcmp(Token,"pastevents")!=0 && strcmp(Token,"pastevents purge")!=0 && strncmp(Token,"pastevents execute",18)!=0 && flag!=1){  

        if (command_count < 15) {
            strcpy(command_history[command_count], Token);
            
        } else {
            // If more than 15 commands are stored, shift them up
    
            for (int i = 0; i < 14; i++) {
                strcpy(command_history[i], command_history[i + 1]);
                //printf("%s<------%s\n",command_history[i], command_history[i + 1]);
            }
            strcpy(command_history[14], Token);
            
        }
        command_count++;
        }
        if((Token,"pastevents execute",18)==0)
        {
            int number;
            sscanf(Token + 19, "%d", &number);
             if (command_count < 15) {
            strcpy(command_history[command_count], abs_path_history[command_count-number]);
            
        } else {
            // If more than 15 commands are stored, shift them up
    
            for (int i = 0; i < 14; i++) {
                strcpy(command_history[i], command_history[i + 1]);
                //printf("%s<------%s\n",command_history[i], command_history[i + 1]);
            }
            strcpy(command_history[14],abs_path_history[command_count-number]);
            
        }
        command_count++;
        }
        
        char*token=strtok(strdup(Token), " \t");
        // printf("Token:%s\n",Token);
        // printf("Token:%s\n",token);
        //while(token!=NULL){
        //     printf("dommy\n");
        if(strcmp(token,"exit")==0)
        {
             valid=4;
            come_out=1;
            break;
            
        }
        else if(strcmp(token,"warp")==0)
        {
             valid=5;
            token = strtok(NULL," \t");
            // if(strcmp(token,";")==0)
            // {
            //     continue;
            // }
            // printf("token:%s <---\n",token);
            if(token==NULL)
            {
                chdir(initial_dir);
                printf("%s\n",initial_dir);
            }
            while(token!=NULL)
            {
                //printf("token:%s &&&&\n",token);
                handleWarpCommand(token,initial_dir,CWD,&history_count,abs_path_history);
               // printf("history_count:%d\n",history_count);
                token= strtok(NULL, "\t");
            }    
        }
        else if(strcmp(token,"peek")==0)
        {
             valid=6;
            token = strtok(NULL, " \t");
            // if(strcmp(token,";")==0)
            // {
            //     continue;
            // }
            //printf("TOKEN:%s\n",token);
            if(token!=NULL)
            {
                handlePeekCommand(token,initial_dir,CWD,&history_count,abs_path_history);
            }
            //printf("dommy\n");
           // handlePeekCommand(input,initial_dir,abs_path_history,history_count);
           //printf("TOKEN:%s\n",token);
        }
        else if(strcmp(token,"pastevents")==0)
        {
             valid=7;
            token = strtok(NULL, " \t");
            //  if(strcmp(token,";")==0)
            // {
            //     continue;
            // }
            if(token ==NULL)
            {
                execute_pastevents(command_history,&command_count);
            }
            else if(strcmp(token,"purge")==0)
            {
                execute_pastevents_purge(command_history,&command_count);
            }
            else if(strcmp(token,"execute")==0)
            {
                token = strtok(NULL, " \t");
                int num = atoi(token);
                //printf("%d and %s\n",num,token);
                execute_pastevents_execute(num,command_history,&command_count,initial_dir,CWD,username,systemname,abs_path_history,&history_count);
            }
        }
        else if(strcmp(token,"proclore")==0)
        { 
                // printf("reached\n");
                //  printf("str_num:-%s-\n",token);
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
                 valid=3;
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
        // printf("INPUT: %s\n",input);
       // printf("TOKN: %s\n",Token);
        char *semicolonPos = strchr(input, ';');
       if (semicolonPos != NULL) {
         strcpy(input,semicolonPos + 1);
       }else 
       {
        break;
        }
        //printf("INPUT: %s\n",input);
        Token=strtok(strdup(input),"\t;\t");
        char *trimmedToken = Token;
        while (*trimmedToken == ' ' || *trimmedToken == '\t') {
        trimmedToken++;
        }
      //  printf("TOKAN: %s\n",trimmedToken);
        Token=trimmedToken;
        }        
        // printf("dobby\n");
        //  }            
    }

    if(come_out==1)
    {
        break;
    }
    
    }
    if(valid==0)
    {
       printf("%s is not a valid command\n",input);
    }
    }
    return 0;
}    

