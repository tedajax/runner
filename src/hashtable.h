#ifndef _HASHTABLE_H_
#define _HASHTABLE_H_

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "types.h"
#include "vector.h"

typedef void(*hashtable_free_f)(void *);

typedef struct hashtable_node_t {
    u64 key;
    void *value;
} HashtableNode;

typedef struct hashtable_t {
    u32 bucketCount;
    hashtable_free_f freeFunc;
    Vector **buckets;
    u32 size;
} Hashtable;

Hashtable *hashtable_new(u32 buckets, hashtable_free_f freeFunc);
void hashtable_init(Hashtable* self, u32 buckets, hashtable_free_f freeFunc);

//insert data into hashtable with given key
//if key does not exist in hash table, inserts value with key and returns true
//otherwise returns false and does not insert value
bool hashtable_insert(Hashtable *self, const char *key, void *data);

//returns value stored with key or NULL if it is not found
void *hashtable_get(Hashtable *self, const char *key);

//removes value stored at key and returns it
void *hashtable_remove(Hashtable *self, const char *key);

size_t hashtable_get_all(Hashtable* self, void** data, size_t n);

void hashtable_clear(Hashtable* self);
void hashtable_free(Hashtable *self);

u64 _hashtable_djb2(const char *key);
u32 _hashtable_index(Hashtable *self, const char *key);

#endif