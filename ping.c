#include"headers.h"

void pingProcess(int pid, int signalNumber, BackgroundProcess*bg_processes,int*num_bg_processes ) {
    // Check if the process exists
    if (kill(pid, 0) == -1) {
        perror("No such process found");
        return;
    }

    // Calculate the actual signal based on the provided signal number (mod 32)
    signalNumber %= 32;

    // Send the specified signal to the process
    if (kill(pid, signalNumber) == -1) {
        perror("Error sending signal");
    } else {
        printf("Sent signal %d to process with pid %d\n", signalNumber, pid);

        // Send the SIGTERM signal (signal number 15) to gracefully terminate the process
        int val=0;
        if(signalNumber==9)
        {
        if (kill(pid, SIGKILL) == -1) {
            perror("Error sending SIGTERM signal");
        }
        val=1;
        }
        else if (signalNumber == 18) 
        { 
            if (kill(pid, SIGCONT) == -1) {
                perror("Error sending SIGCONT signal");
            }
            val=2;
        } 
        else if (signalNumber == 19) 
        {
              if (kill(pid, SIGSTOP) == -1) {
                perror("Error sending SIGSTOP signal");
            }
            val=3;
        } 
        else if (signalNumber == 20) {
            if (kill(pid, SIGTSTP) == -1) {
                perror("Error sending SIGTSTP signal");
             }
             val=4;
        }
        for(int i=0;i<*num_bg_processes;i++)
        {
            if(bg_processes[i].pid==pid)
            {
                if(val==1){
                bg_processes[i].state=-1;
                }
                else if(val==2)
                {
                bg_processes[i].state=1;
                }
                else if(val==3 || val==4)
                {
                    bg_processes[i].state=0; 
                }
            }
        }
    }
}
