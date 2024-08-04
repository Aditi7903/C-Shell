#ifndef __WARP_H
#define __WARP_H
void changeDirectoryAndUpdateHistory(const char* new_dir, const char* initial_dir,
                                     char abs_path_history[][1024], int*  history_count) ;

void handleWarpCommand(const char* input, const char* initial_dir,char*cwd,int* history_count,char abs_path_history[][1024]);
       
#endif