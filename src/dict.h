#ifndef DICT_H
#define DICT_H

#include "types.h"
#include <stdlib.h>

// Really barebones key, value store
// Keys must be uint32_t, values are stored in a void*

extern const u32 DICT_INVALID_KEY;

typedef struct dict_list_node_t {
    void* element;
    struct dict_list_node_t* next;
    struct dict_list_node_t* prev;
} DictListNode;

typedef struct dict_node_t {
    u32 key;
    DictListNode* list;
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
DictListNode* dict_remove(Dictionary* self, u32 key);
void* dict_get(Dictionary* self, u32 key, u32 index);
DictListNode* dict_get_all(Dictionary* self, u32 key);

void dict_list_free(DictListNode* self);

#endif