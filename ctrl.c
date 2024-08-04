#include"headers.h"

void sigint_handler(int signum) {
    if (foreground_pid != -1) {
        // Send SIGINT to the currently running foreground process
        kill(foreground_pid, SIGINT);
    } else {
        // No foreground process running, so do nothing
    }
}



void sigtstp_handler(int signum) {
    // Handle Ctrl+Z (SIGTSTP) signal
    if (is_foreground) {
        if (foreground_pid > 0) {
            // Send SIGTSTP to the foreground process group
            kill(-foreground_pid, SIGTSTP);
            printf("Process %d stopped\n", foreground_pid);

            // // Update the background process struct
            // int i;
            // for (i = 0; i < 10; i++) {
            //     if (bg_processes[i].pid == -1) {
            //         bg_processes[i].pid = foreground_pid;
            //         strcpy(bg_processes[i].command, "unknown"); // You can set the command appropriately.
            //         bg_processes[i].state = 0; // Set the state to stopped
            //         break;
            //     }
            // }
        }
    }
}


