#include "../directory.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

Directory* platform_directory_open(const char* path) {
    WIN32_FIND_DATA fdFile;
    HANDLE hFind = NULL;

    char sPath[2048];

    sprintf(sPath, "%s\\*.*", path);

    if ((hFind = FindFirstFile(sPath, &fdFile)) == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Directory not found: \'%s\'\n", path);
        return NULL;
    }

    Directory* result = (Directory*)calloc(1, sizeof(Directory));
    result->path = (char*)calloc(strlen(path) + 1, sizeof(char));
    result->count = 0;
    result->current = 0;
    sprintf(result->path, "%s", path);
    int capacity = 16;
    result->files = (FileDescriptor*)calloc(capacity, sizeof(FileDescriptor));

    do {
        if (strcmp(fdFile.cFileName, ".") == 0 || strcmp(fdFile.cFileName, "..") == 0) {
            continue;
        }

        if (result->count >= capacity) {
            capacity *= 2;
            result->files = (FileDescriptor*)realloc(result->files, sizeof(FileDescriptor) * capacity);
        }

        if (fdFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            // Do nothing
        } else {
            result->files[result->count].filename = (char*)calloc(260, sizeof(char));
            sprintf(result->files[result->count].filename, "%s", fdFile.cFileName);
            result->files[result->count].index = result->count;
            ++result->count;
        }        
    } while (FindNextFile(hFind, &fdFile));

    FindClose(hFind);

    return result;
}
