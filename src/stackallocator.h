#ifndef RUNNER_STACK_ALLOCATOR_H
#define RUNNER_STACK_ALLOCATOR_H

#include <stdlib.h>
#include "types.h"
#include "debug.h"

typedef struct stack_allocator_t {
    void* block;
    size_t size;
    size_t head;
} StackAllocator;

void stackor_init(StackAllocator* self, size_t bytes);
void stackor_delete(StackAllocator* self);
void* stackor_alloc(StackAllocator* self, size_t bytes);
void* stackor_get(StackAllocator* self, size_t offset);
void stackor_free(StackAllocator* self, size_t bytes);
void stackor_free_to(StackAllocator* self, void* location);
void stackor_clear(StackAllocator* self);

#define STACKOR_GET_TYPE(allocator, type, offset) ((type)stackor_get(allocator, offset));

#endif