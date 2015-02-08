#include "../directory.h"

#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include "memory.h"

//TODO: need to implement this on the linux side and get the makefile ignoring the \Win directory

Directory* platform_directory_open(const char* path) {
    DIR *dir;
    struct dirent* ent;
    if ((dir = opendir(path)) != NULL){
        Directory* result = (Directory*)calloc(1, sizeof(Directory));
        result->path = (char*)calloc(strlen(path) + 1, sizeof(char));
        result->count = 0;
        result->current = 0;
        sprintf(result->path, "%s", path);
        int capacity = 16;
        result->files = (FileDescriptor*)calloc(capacity, sizeof(FileDescriptor));

        while ((ent = readdir(dir)) != NULL) {
            if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {
                continue;
            }

            if (result->count >= capacity) {
                capacity *= 2;
                result->files = (FileDescriptor*)realloc(result->files, sizeof(FileDescriptor) * capacity);
            }

            if (ent->d_type == DT_REG) {
                result->files[result->count].filename = (char*)calloc(260, sizeof(char));
                sprintf(result->files[result->count].filename, "%s", ent->d_name);
                result->files[result->count].index = result->count;
                ++result->count;
            }
        }
        closedir(dir);

        return result;
    } else {
        fprintf(stderr, "Directory not found: \'%s\'\n", path);
        return NULL;
    }
}
