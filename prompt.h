#ifndef __PROMPT_H
#define __PROMPT_H

char *get_username();

char *get_system_name();

char *get_relative_path(char *cwd, const char initial_dir[1024]);

#endif
