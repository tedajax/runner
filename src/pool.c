#include "pool.h"
#include <memory.h>
#include <stdlib.h>

void pool_index_stack_resize(PoolIndexStack* self, u32 capacity);

void pool_index_stack_init(PoolIndexStack* self, u32 capacity) {
    self->capacity = capacity;
    self->count = 0;
    self->indices = CALLOC(self->capacity, i32);
    for (u32 i = 0; i < self->capacity; ++i) {
        self->indices[i] = -1;
    }
}

void pool_index_stack_push(PoolIndexStack* self, i32 index) {
    ASSERT(self->count < self->capacity, "Tried to push too many stack indices on pool stack.  Pool stack should be resized.");

    self->indices[self->count] = index;
    ++self->count;
}

i32 pool_index_stack_pop(PoolIndexStack* self) {
    ASSERT(self->count > 0, "Attempt to pop empty stack.");

    i32 result = self->indices[self->count - 1];
    self->indices[self->count - 1] = -1;
    --self->count;
    return result;
}

i32 pool_index_stack_peek(PoolIndexStack* self) {
    ASSERT(self->count > 0, "Nothing to peek.");
    return self->indices[self->count - 1];
}

void pool_index_stack_resize(PoolIndexStack* self, u32 capacity) {
    self->capacity = capacity;
    self->indices = (i32*)realloc(self->indices, sizeof(i32) * self->capacity);
}