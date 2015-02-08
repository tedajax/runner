#ifndef RUNNER_DIRECTORY_H
#define RUNNER_DIRECTORY_H

#include "memory.h"

typedef struct file_descriptor_t {
    int index;
    char* filename;
} FileDescriptor;

typedef struct directory_t {
    char* path;
    FileDescriptor* files;
    int count;
    int current;
} Directory;

Directory* platform_directory_open(const char* path);

Directory* directory_open(const char* path);
void directory_close(Directory* self);
FileDescriptor* directory_next(Directory* self);

#endif