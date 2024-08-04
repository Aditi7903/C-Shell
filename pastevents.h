#ifndef __PASTEVENTS_H
#define __PASTEVENTS_H

void execute_pastevents(char command_history[][1024],int*command_count);

void execute_pastevents_purge(char command_history[][1024],int*command_count);

void execute_the_command(char*input,char initial_dir[1024],char*cwd,char abs_path_history[][1024],int*history_count);

void execute_pastevents_execute(int num,char command_history[][1024],int*command_count,char initial_dir[1024],char*cwd,char*username,char*systemname,char abs_path_history[][1024],int*history_count);

#endif

