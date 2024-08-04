#include "headers.h"

void print_process_info(const char *pid_str) {
    char proc_path[path_length];
    snprintf(proc_path, sizeof(proc_path), "/proc/%s/status", pid_str);

    FILE *proc_file = fopen(proc_path, "r");
    if (proc_file == NULL) {
        perror("Error opening process status file");
        return;
    }

    char line[line_length];
    char status;
    char process_group[line_length];
    char virtual_memory[line_length];
    char executable_path[path_length];

    while (fgets(line, sizeof(line), proc_file) != NULL) {
        if (sscanf(line, "State:\t%c", &status) == 1) {
            // Process status
            printf("process status: %c", status);
            if (status == 'R' || status == 'S' || status == 'Z') {
                if (status == 'R' && strcmp(pid_str, "1") != 0) {
                    printf("+");
                }
                printf("\n");
            } else {
                printf(" (unknown)\n");
            }
        } else if (sscanf(line, "PPid:\t%s", process_group) == 1) {
            // Process group
            printf("process Group: %s\n", process_group);
        } else if (sscanf(line, "VmSize:\t%s", virtual_memory) == 1) {
            // Virtual memory
            printf("Virtual memory: %s\n", virtual_memory);
        }
    }

    fclose(proc_file);

    snprintf(proc_path, sizeof(proc_path), "/proc/%s/exe", pid_str);
    ssize_t len = readlink(proc_path, executable_path, sizeof(executable_path) - 1);
    if (len != -1) {
        executable_path[len] = '\0';
        printf("executable path: %s\n", executable_path);
    }
}

