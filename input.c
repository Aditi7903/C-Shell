#include "headers.h"

void runInBackground(const char *command) {
    pid_t bg_pid = fork();
    if (bg_pid == 0) {
        printf("%d\n", getpid());
        
        // Redirect standard error to /dev/null
        int dev_null = open("/dev/null", O_WRONLY);
        if (dev_null != -1) {
            printf("ERROR: '%s' is not a valid command\n", command);
            dup2(dev_null, 2);
            close(dev_null);
        }
        exit(0);
    }
}
void runInForeground(const char *command) {
   // printf("Running in foreground: %s\n", command);
    int result=system(command);
    if (result == -1) {
        printf("ERROR: '%s' is not a valid command\n", command);
    }
}