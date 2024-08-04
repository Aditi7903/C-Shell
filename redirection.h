#ifndef __REDIRECTION_H
#define __REDIRECTION_H

void ExecuteCommand(Command *cmd);

void execute_shell_command(ShellCommand *command, int input_fd, int output_fd);

#endif
