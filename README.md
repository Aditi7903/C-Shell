[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/76mHqLr5)
# Description
1. main.c ->  It handles user input and commands, including both user-defined and system-defined commands. It manages the execution of foreground and background processes and is the entry point of your program.
2. prompt.c -> This file includes a function responsible for determining and printing the relative path in my prompt. It assists in displaying the current directory in a user-friendly way.
3. input.c -> This file contains a function to execute the foreground and background processes separated by '&'.
4. warp.c -> This file contains a function to implement the warp command. The command's functionality might involve navigating or changing the current working directory based on the flags used.
5. peek.c ->  This file seems to house a function to display files and their details based on flags provided. It's likely related to listing and showing information about files and directories.
6. pastevents.c -> This file likely contains a function to display the past events and also execute commands from the past events. It retrieves and runs previously executed commands based on user input, also contains a function to remove the history of past events.
7. system.c -> This contains functions to handle the foreground and background processes (ending with '&'). (system commands) 
8. proclore.c -> This file appears to contain a function that implements the proclore command. This command retrieves and displays information about a process, including its status, process group,      virtual memory, and executable path.
9. seek.c -> This file probably implements the seek command. It may include functions to search for specific content within files based on flags provided by the user and seems to house a recursive function that searches for a specific file or folder name, likely within a directory hierarchy.
10. resolveAndOpenSocket -> Implement the function for fetching manpages of commands.
11. ctrl.c:  handleCtrlC - Implement the function for handling Ctrl+C ,handleCtrlD - Implement the function for handling Ctrl+D, handleCtrlZ - Implement the function for handling Ctrl+Z.
12. ping.c: sendSignalToProcess -> Implement the function for sending a signal to a process with a specified PID.
13. redirection.c -> Implement the functions for input and output redirection as needed.
14. neonate.c -> Implement the function for printing the PID of the most recent process in your shell at regular intervals until a specific key (e.g., 'x') is pressed.
15. activities.c -> Implement the function for printing the processes spawned by your shell.
# Assumptions 
