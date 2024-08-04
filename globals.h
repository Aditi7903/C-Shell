#ifndef __GLOBALS_H
#define __GLOBALS_H
typedef struct cm {
    char cmd[MAX_INPUT_LENGTH];
    char infile[MAX_INPUT_LENGTH];
    char outfile[MAX_INPUT_LENGTH];
    bool append;
} Command;

typedef struct bg {
    pid_t pid;
    char command[1024];
    int state; // 0 for stopped , 1 for running , -1 for terminated
} BackgroundProcess;

typedef struct shellcm {
    char command[MAX_INPUT_LENGTH];
} ShellCommand;

extern struct termios orig_termios;

extern bool is_foreground;
extern pid_t foreground_pid;

#endif
