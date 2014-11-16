#ifndef DICT_H
#define DICT_H

#include "types.h"
#include <stdlib.h>

// Really barebones key, value store
// Keys must be uint32_t, values are stored in a void*

extern const u32 DICT_INVALID_KEY;

typedef struct dict_node_t {
    u32 key;
    void* element;
    struct dict_node_t* next;
} DictionaryNode;

typedef struct dict_t {
    DictionaryNode* buckets;
    size_t bucketCount;
    size_t size;
} Dictionary;

Dictionary* dict_new(size_t buckets);
void dict_init(Dictionary* self, size_t buckets);
void dict_set(Dictionary* self, u32 key, void* element);
void* dict_remove(Dictionary* self, u32 key);
void* dict_get(Dictionary* self, u32 key);

#endif