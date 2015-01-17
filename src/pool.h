#ifndef RUNNER_POOL_H
#define RUNNER_POOL_H

#include "types.h"
#include "debug.h"

typedef struct pool_index_stack_t {
    u32 capacity;
    u32 count;
    i32* indices;
} PoolIndexStack;

void pool_index_stack_init(PoolIndexStack* self, u32 capacity);
void pool_index_stack_push(PoolIndexStack* self, i32 index);
i32 pool_index_stack_pop(PoolIndexStack* self);
i32 pool_index_stack_peek(PoolIndexStack* self);

typedef int(*pool_cmp_f)(void* a, void* b);

#define POOL(type) Pool##type

#define POOL_DEFINE(type)                   \
    typedef struct pool_##type##_t {        \
        u32 capacity;                       \
        PoolIndexStack indexStack;          \
        type** data;                        \
    } POOL(type);


#define POOL_INIT(type) pool_##type##_init
#define POOL_INSERT(type) pool_##type##_insert
#define POOL_GET(type) pool_##type##_get
#define POOL_REMOVE_AT(type) pool_##type##_remove_at
#define POOL_FREE(type) pool_##type##_free

#define POOL_INIT_PROTO(type) void POOL_INIT(type)(POOL(type)* self, u32 capacity)
#define POOL_INSERT_PROTO(type) type* POOL_INSERT(type)(POOL(type)* self, type* element)
#define POOL_GET_PROTO(type) type* POOL_GET(type)(POOL(type)* self, u32 index)
#define POOL_REMOVE_AT_PROTO(type) type* POOL_REMOVE_AT(type)(POOL(type)* self, u32 index)
#define POOL_FREE_PROTO(type) void POOL_FREE(type)(POOL(type)* self)

#define POOL_INIT_IMPL(type) POOL_INIT_PROTO(type) {                            \
    self->capacity = capacity;                                                  \
    pool_index_stack_init(&self->indexStack, self->capacity);                   \
    for (i32 i = self->capacity - 1; i >= 0; --i) {                             \
        pool_index_stack_push(&self->indexStack, i);                            \
        }                                                                           \
    self->data = CALLOC(self->capacity, type*);                                 \
}

#define POOL_INSERT_IMPL(type) POOL_INSERT_PROTO(type) {                        \
    i32 index = pool_index_stack_pop(&self->indexStack);                        \
    ASSERT(index >= 0, "Could not get a valid index");                          \
    self->data[index] = element;                                                \
    return element;                                                             \
}

#define POOL_GET_IMPL(type) POOL_GET_PROTO(type) {                              \
    ASSERT(index < self->capacity, "Index out of pool range.");                 \
    return self->data[index];                                                   \
}

#define POOL_REMOVE_AT_IMPL(type) POOL_REMOVE_AT_PROTO(type) {                  \
    ASSERT(index < self->capacity, "Index out of pool range.");                 \
    type* result = self->data[index];                                           \
    self->data[index] = NULL;                                                   \
    pool_index_stack_push(&self->indexStack, index);                            \
    return result;                                                              \
}

#define POOL_FREE_IMPL(type) POOL_FREE_PROTO(type) {                            \
    free(self->indexStack.indices);                                             \
    for (u32 i = 0; i < self->capacity; ++i) {                                  \
        if (self->data[i]) { free(self->data[i]); }                             \
    }                                                                           \
}

#define POOL_REGISTER(type)     \
    POOL_DEFINE(type);          \
    POOL_INIT_PROTO(type);      \
    POOL_INSERT_PROTO(type);    \
    POOL_GET_PROTO(type);       \
    POOL_REMOVE_AT_PROTO(type); \
    POOL_FREE_PROTO(type);

#define POOL_IMPLEMENTATION(type)   \
    POOL_INIT_IMPL(type)            \
    POOL_INSERT_IMPL(type)          \
    POOL_GET_IMPL(type)             \
    POOL_REMOVE_AT_IMPL(type)       \
    POOL_FREE_IMPL(type)

#endif