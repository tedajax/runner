#ifndef _HASHSET_H_
#define _HASHSET_H_

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "types.h"
#include "vector.h"

typedef struct hashset_node_t {
    u64 key;
    bool value;
} HashsetNode;

typedef struct hashset_t {
    u32 bucketCount;
    Vector **buckets;
    u32 size;
} Hashset;

Hashset *hashset_new(u32 buckets);
void hashset_init(Hashset* self, u32 buckets);

void hashset_add(Hashset *self, const char *key);
bool hashset_contains(Hashset *self, const char *key);
void hashset_remove(Hashset *self, const char *key);

void hashset_free(Hashset *self);

u64 _hashset_djb2(const char *key);
u32 _hashset_index(Hashset *self, const char *key);

#endif