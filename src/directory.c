#include "directory.h"

Directory* directory_open(const char* path) {
    return platform_directory_open(path);
}

void directory_close(Directory* self) {
    free(self->path);
    for (int i = 0; i < self->count; ++i) {
        free(self->files->filename);
    }
    free(self->files);
    free(self);
}

FileDescriptor* directory_next(Directory* self) {
    if (!self) {
        return NULL;
    }

    if (self->current >= self->count) {
        self->current = 0;
        return NULL;
    }

    FileDescriptor* result = &self->files[self->current];
    ++self->current;
    return result;
}