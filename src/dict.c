#include "dict.h"
#include "debug.h"

const u32 DICT_INVALID_KEY = 0;

Dictionary* dict_new(dict_free_f freeFunc) {
    Dictionary* self = (Dictionary*)calloc(1, sizeof(Dictionary));

    dict_init(self, freeFunc);

    return self;
}

void dict_init(Dictionary* self, dict_free_f freeFunc) {
    for (u32 i = 0; i < DICT_BUCKET_COUNT; ++i) {
        for (u32 j = 0; j < DICT_MAX_ELEMENTS_PER_BUCKET; ++j) {
            self->buckets[i][j].key = DICT_INVALID_KEY;
            self->buckets[i][j].list = NULL;
        }
    }
    self->size = 0;
    self->freeFunc = freeFunc;
}

void dict_set(Dictionary* self, u32 key, void* element) {
    if (!self) {
        return;
    }

    if (key == DICT_INVALID_KEY) {
        return;
    }

    u32 bucket = key & DICT_BUCKET_COUNT_MASK;
    u32 index = 0;

    DictionaryNode* node = &self->buckets[bucket][index];

    while (node->key != DICT_INVALID_KEY) {
        if (node->key == key) {
            DictListNode* prev = NULL;
            DictListNode* lnode = node->list;
            while (lnode->element != NULL) {
                prev = lnode;
                lnode = lnode->next;
            }
            lnode->element = element;
            lnode->prev = prev;
            ++self->size;
            return;
        }
                
        ASSERT(index < DICT_MAX_ELEMENTS_PER_BUCKET - 1, "Reached bucket limit for dictionary, consider increasing DICT_MAX_ELEMENTS_PER_BUCKET.");
        ++index;
        node = &self->buckets[bucket][index];
    }

    node->key = key;
    node->list = (DictListNode*)calloc(1, sizeof(DictListNode));
    node->list->next = NULL;
    node->list->prev = NULL;
    node->list->element = element;
    ++self->size;
}

DictListNode* dict_remove(Dictionary* self, u32 key) {
    if (!self) {
        return NULL;
    }

    u32 bucket = key & DICT_BUCKET_COUNT_MASK;

    void* result = NULL;

    for (u32 i = 0; i < DICT_MAX_ELEMENTS_PER_BUCKET; ++i) {
        DictionaryNode* node = &self->buckets[bucket][i];
        if (node->key == key) {
            result = node->list;
            node->key = DICT_INVALID_KEY;
            node->list = NULL;
            --self->size;
            break;
        }
    }

    return result;
}

void dict_clear(Dictionary* self) {
    for (u32 i = 0; i < DICT_BUCKET_COUNT; ++i) {
        for (u32 j = 0; j < DICT_MAX_ELEMENTS_PER_BUCKET; ++j) {
            DictionaryNode* node = &self->buckets[i][j];
            DictListNode* lnode = node->list;
            while (lnode != NULL) {
                if (lnode->element) {
                    if (self->freeFunc) {
                        self->freeFunc(lnode->element);
                    } else {
                        free(lnode->element);
                    }
                }
                lnode = lnode->next;
            }
            if (node->list) {
                free(node->list);
                node->list = NULL;
            }
        }
    }
}

void* dict_get(Dictionary* self, u32 key, u32 index) {
    DictListNode* list = dict_get_all(self, key);

    if (list) {
        u32 i = 0;
        while (list != NULL && i < index) {
            list = list->next;
            ++i;
        }

        return list->element;
    }

    return NULL;
}

DictListNode* dict_get_all(Dictionary* self, u32 key) {
    if (!self) {
        return NULL;
    }

    u32 bucket = key & DICT_BUCKET_COUNT_MASK;

    for (u32 i = 0; i < DICT_MAX_ELEMENTS_PER_BUCKET; ++i) {
        DictionaryNode* node = &self->buckets[bucket][i];
        if (node->key == key) {
            return node->list;
        }
    }

    return NULL;
}

void dict_list_free(DictListNode* self) {
    DictListNode* prev = NULL;
    DictListNode* node = self;
    while (node != NULL) {
        prev = node;
        node = node->next;
        if (prev) {
            free(prev);
        }
    }
}
