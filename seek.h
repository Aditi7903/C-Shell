#ifndef __SEEK_H
#define __SEEK_H

void listFilesInDirectory(const char *dirPath, const char *search, int isFile, int isDir, const char *basepath, int *countf, int *countd, char *changedir);

void seekCommand(const char *search, const char *targetDir, int isFile, int isDir, int Eflag);

#endif