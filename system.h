#ifndef __SYSTEM_H
#define __SYSTEM_H

void RunInForeground(char *command ,char*username,char*systemname,char*initial_dir);

void RunInBackground(const char *command, BackgroundProcess bg_processes[], int *num_bg_processes);

#endif