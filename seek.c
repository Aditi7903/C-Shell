#include "headers.h"

void listFilesInDirectory(const char *dirPath, const char *search, int isFile, int isDir, const char *basepath, int *countf, int *countd, char *changedir) {
    DIR *dir = opendir(dirPath);
    if (dir == NULL) {
        perror("opendir");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char filePath[PATH_MAX];
        snprintf(filePath, sizeof(filePath), "%s/%s", dirPath, entry->d_name);

        struct stat fileInfo;
        if (stat(filePath, &fileInfo) == -1) {
            perror("stat");
            continue;
        }
        char list_arr[256];
        char list_arr1[256];
         if(isFile == 0 && isDir == 0){
            if (S_ISREG(fileInfo.st_mode)) {
                if (strcmp(entry->d_name, search) == 0) {
                    (*countf)++;
                    list_arr[0] = '.';
                    strcpy(list_arr + 1, filePath + strlen(basepath));
                    printf("\033[0;32m%s\033[0m\n", list_arr);
                    strcpy(changedir, filePath);
                }
            } else if (S_ISDIR(fileInfo.st_mode)) {
                if (strcmp(entry->d_name, search) == 0) {
                    (*countd)++;
                    list_arr1[0] = '.';
                    strcpy(list_arr1 + 1, filePath + strlen(basepath));
                    printf("\033[0;34m%s\033[0m\n", list_arr1);
                    strcpy(changedir, filePath);
                }
                listFilesInDirectory(filePath, search, isFile, isDir, basepath, countf, countd, changedir);
            }
         } else if(isFile == 0 && isDir == 1){
            if (S_ISREG(fileInfo.st_mode)) {
                if (strcmp(entry->d_name, search) == 0) {
                    //printf("\033[0;32m%s\033[0m\n", filePath);
                }
            } else if (S_ISDIR(fileInfo.st_mode)) {
                if (strcmp(entry->d_name, search) == 0) {
                    (*countd)++;
                    list_arr1[0] = '.';
                    strcpy(list_arr1 + 1, filePath + strlen(basepath));
                    printf("\033[0;34m%s\033[0m\n", list_arr1);
                    strcpy(changedir, filePath);
                }
                listFilesInDirectory(filePath, search, isFile, isDir, basepath, countf, countd, changedir);
            }
         } else if(isFile == 1 && isDir == 0){
             if (S_ISREG(fileInfo.st_mode)) {
                if (strcmp(entry->d_name, search) == 0 ) {
                    (*countf)++;
                    list_arr[0] = '.';
                    strcpy(list_arr + 1, filePath + strlen(basepath));
                    printf("\033[0;32m%s\033[0m\n", list_arr);
                    strcpy(changedir, filePath);
                }
            } else if (S_ISDIR(fileInfo.st_mode)) {
                if (strcmp(entry->d_name, search) == 0) {
                    //printf("\033[0;34m%s\033[0m\n", filePath);
                }
                listFilesInDirectory(filePath, search, isFile, isDir, basepath, countf, countd, changedir);
            }
         }
    }

    closedir(dir);
}


void seekCommand(const char *search, const char *targetDir, int isFile, int isDir, int Eflag) {
    if (isFile == 1 && isDir == 1) {
        printf("Invalid flags!\n");
        return;
    }
    int countf = 0;
    int countd = 0;
    char changedir[256];
    listFilesInDirectory(targetDir, search, isFile, isDir, targetDir, &countf, &countd, changedir);
    if (countf == 0 && countd == 0) {
        printf("No match found!\n");
    }
    if (countf == 0 && countd == 1 && Eflag == 1) {
        if (access(changedir, X_OK) == -1) {
            perror("access");
            printf("Missing permissions for task!\n");
        }
        if (chdir(changedir) == -1) {
            perror("chdir");
        }
    }
    if (countf == 1 && countd == 0 && Eflag == 1) {
        char write2[256];
        if (getcwd(write2, sizeof(write2)) != NULL) {
            char *write;
            write = strrchr(changedir, '/');
            changedir[strlen(changedir) - strlen(write)] = '\0';
            chdir(changedir);
            char write1[256];
            strcpy(write1, write + 1);
            FILE *file = fopen(write1, "r");
            if (file == NULL) {
                perror("fopen");
            }
            char buffer[1024];
            while (fgets(buffer, sizeof(buffer), file) != NULL) {
                printf("%s\n", buffer);
            }
            fclose(file);
        }
        chdir(write2);
    }
}