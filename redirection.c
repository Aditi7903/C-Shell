#include"headers.h"

void ExecuteCommand(Command *cmd) {
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Child process
        int in_fd, out_fd;

        // Handle input redirection
        if (cmd->infile[0] != '\0') {
            in_fd = open(cmd->infile, O_RDONLY);
            if (in_fd == -1) {
                perror("Error opening input file");
                exit(EXIT_FAILURE);
            }
            dup2(in_fd, STDIN_FILENO);
            close(in_fd);
        }

        // Handle output redirection
        if (cmd->outfile[0] != '\0') {
            if (cmd->append) {
                out_fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
            } else {
                out_fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            }
            if (out_fd == -1) {
                perror("Error opening output file");
                exit(EXIT_FAILURE);
            }
            dup2(out_fd, STDOUT_FILENO);
            close(out_fd);
        }

        // Execute the command
        system(cmd->cmd);

        exit(EXIT_SUCCESS);
    } else {
        // Parent process
        int status;
        waitpid(pid, &status, 0);
    }
}

void execute_shell_command(ShellCommand *command, int input_fd, int output_fd) {
    pid_t child_pid = fork();
    if (child_pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (child_pid == 0) {
        // Child process

        // Redirect stdin and stdout
        if (input_fd != STDIN_FILENO) {
            dup2(input_fd, STDIN_FILENO);
            close(input_fd);
        }

        if (output_fd != STDOUT_FILENO) {
            dup2(output_fd, STDOUT_FILENO);
            close(output_fd);
        }

        // Execute the command
        execlp(command->command, command->command, (char *)NULL);
        perror("exec");
        exit(EXIT_FAILURE);
    } else {
        // Parent process
        int status;
        waitpid(child_pid, &status, 0);
    }
}
