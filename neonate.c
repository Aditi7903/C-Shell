#include"headers.h"

void die(const char *s) {
    perror(s);
    exit(1);
}


void disableRawMode() {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die("tcsetattr");
}

void enableRawMode() {
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) die("tcgetattr");
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
}

void neonate(int time_arg) {
    enableRawMode();
    printf("# Press 'x' to exit\n");

    while (1) {
        // Print the most recently created process ID (PID)
        printf("%d\n", getpid());
        fflush(stdout);

        // Sleep for the specified time
        sleep(time_arg);

        // Check for user input ('x' keypress)
        char c;
        if (read(STDIN_FILENO, &c, 1) == 1 && c == 'x') {
            break;
        }
        else if(c==4)
        {
            printf("Ctrl+D detected. Logging out...\n");
            exit(0);
        }
    }

    disableRawMode();
}
