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
        u32 count;                          \
        PoolIndexStack indexStack;          \
        type* data;                         \
        type invalidValue;                  \
    } POOL(type);

#define POOL_DEFINE_NAMED(type, name)       \
    typedef struct pool_##name##_t {        \
        u32 capacity;                       \
        u32 count;                          \
        PoolIndexStack indexStack;          \
        type* data;                         \
        type invalidValue;                  \
    } POOL(name);

#define POOL_INIT(type) pool_##type##_init
#define POOL_INSERT(type) pool_##type##_insert
#define POOL_GET(type) pool_##type##_get
#define POOL_REMOVE_AT(type) pool_##type##_remove_at
#define POOL_FREE(type) pool_##type##_free

#define POOL_INIT_PROTO(type) void POOL_INIT(type)(POOL(type)* self, u32 capacity, type invalid)
#define POOL_INSERT_PROTO(type) type* POOL_INSERT(type)(POOL(type)* self, type element)
#define POOL_GET_PROTO(type) type* POOL_GET(type)(POOL(type)* self, u32 index)
#define POOL_REMOVE_AT_PROTO(type) type POOL_REMOVE_AT(type)(POOL(type)* self, u32 index)
#define POOL_FREE_PROTO(type) void POOL_FREE(type)(POOL(type)* self)

#define POOL_INIT_NAMED_PROTO(type, name) void POOL_INIT(name)(POOL(name)* self, u32 capacity, type invalid)
#define POOL_INSERT_NAMED_PROTO(type, name) type* POOL_INSERT(name)(POOL(name)* self, type element)
#define POOL_GET_NAMED_PROTO(type, name) type* POOL_GET(name)(POOL(name)* self, u32 index)
#define POOL_REMOVE_AT_NAMED_PROTO(type, name) type POOL_REMOVE_AT(name)(POOL(name)* self, u32 index)
#define POOL_FREE_NAMED_PROTO(type, name) void POOL_FREE(name)(POOL(name)* self)

#define POOL_INIT_BODY(type)                                                    \
    self->capacity = capacity;                                                  \
    self->count = 0;                                                            \
    pool_index_stack_init(&self->indexStack, self->capacity);                   \
    for (i32 i = self->capacity - 1; i >= 0; --i) {                             \
        pool_index_stack_push(&self->indexStack, i);                            \
    }                                                                           \
    self->data = CALLOC(self->capacity, type);                                  \
    self->invalidValue = invalid;

#define POOL_INSERT_BODY(type)                                                  \
    i32 index = pool_index_stack_pop(&self->indexStack);                        \
    ASSERT(index >= 0, "Could not get a valid index");                          \
    self->data[index] = element;                                                \
    ++self->count;                                                              \
    return &self->data[index];

#define POOL_GET_BODY(type)                                                     \
    ASSERT(index < self->capacity, "Index out of pool range.");                 \
    return &self->data[index];

#define POOL_REMOVE_AT_BODY(type)                                               \
    ASSERT(index < self->capacity, "Index out of pool range.");                 \
    type result = self->data[index];                                            \
    self->data[index] = self->invalidValue;                                     \
    pool_index_stack_push(&self->indexStack, index);                            \
    --self->count;                                                              \
    return result;

#define POOL_FREE_BODY(type)                                                    \
    free(self->indexStack.indices);

#define POOL_INIT_IMPL(type) POOL_INIT_PROTO(type) { POOL_INIT_BODY(type); }
#define POOL_INSERT_IMPL(type) POOL_INSERT_PROTO(type) { POOL_INSERT_BODY(type); }
#define POOL_GET_IMPL(type) POOL_GET_PROTO(type) { POOL_GET_BODY(type); }
#define POOL_REMOVE_AT_IMPL(type) POOL_REMOVE_AT_PROTO(type) { POOL_REMOVE_AT_BODY(type); }
#define POOL_FREE_IMPL(type) POOL_FREE_PROTO(type) { POOL_FREE_BODY(type); }

#define POOL_INIT_NAMED_IMPL(type, name) POOL_INIT_NAMED_PROTO(type, name) { POOL_INIT_BODY(type); }
#define POOL_INSERT_NAMED_IMPL(type, name) POOL_INSERT_NAMED_PROTO(type, name) { POOL_INSERT_BODY(type); }
#define POOL_GET_NAMED_IMPL(type, name) POOL_GET_NAMED_PROTO(type, name) { POOL_GET_BODY(type); }
#define POOL_REMOVE_AT_NAMED_IMPL(type, name) POOL_REMOVE_AT_NAMED_PROTO(type, name) { POOL_REMOVE_AT_BODY(type); }
#define POOL_FREE_NAMED_IMPL(type, name) POOL_FREE_NAMED_PROTO(type, name) { POOL_FREE_BODY(type); }

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

#define POOL_REGISTER_NAMED(type, name)     \
    POOL_DEFINE_NAMED(type, name);          \
    POOL_INIT_NAMED_PROTO(type, name);      \
    POOL_INSERT_NAMED_PROTO(type, name);    \
    POOL_GET_NAMED_PROTO(type, name);       \
    POOL_REMOVE_AT_NAMED_PROTO(type, name); \
    POOL_FREE_NAMED_PROTO(type, name);

#define POOL_IMPLEMENTATION_NAMED(type, name)           \
    POOL_INIT_NAMED_IMPL(type, name);                   \
    POOL_INSERT_NAMED_IMPL(type, name);                 \
    POOL_GET_NAMED_IMPL(type, name);                    \
    POOL_REMOVE_AT_NAMED_IMPL(type, name);              \
    POOL_FREE_NAMED_IMPL(type, name);

#endif