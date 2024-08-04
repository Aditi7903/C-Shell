#ifndef __PEEK_H
#define __PEEK_H

char *entry_permissions(mode_t mode);

void print_file_details(struct dirent *entry, char *abs_path);

void get_the_list_of_files(const char *full_path, bool show_details, bool show_hidden);

void handlePeekCommand(char* input, const char* initial_dir,const char* cwd, int* history_count,char abs_path_history[][1024]);

#endif