#include "dict.h"

const u32 DICT_INVALID_KEY = 0;

Dictionary* dict_new(dict_free_f freeFunc) {
    Dictionary* self = (Dictionary*)calloc(1, sizeof(Dictionary));

    dict_init(self, freeFunc);

    return self;
}

void dict_init(Dictionary* self, dict_free_f freeFunc) {
    for (u32 i = 0; i < DICT_BUCKET_COUNT; ++i) {
        self->buckets[i].key = DICT_INVALID_KEY;
        self->buckets[i].list = NULL;
        self->buckets[i].next = NULL;
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

    u32 index = key % DICT_BUCKET_COUNT;

    DictionaryNode* prev = NULL;
    DictionaryNode* node = &self->buckets[index];

    if (node->key == DICT_INVALID_KEY) {
        node->key = key;
        node->next = NULL;
        node->list = (DictListNode*)calloc(1, sizeof(DictListNode));
        node->list->next = NULL;
        node->list->prev = NULL;
        node->list->element = NULL;
    }

    while (node != NULL && node->key != DICT_INVALID_KEY) {
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
        prev = node;
        node = node->next;
    }

    DictionaryNode* newNode = (DictionaryNode*)calloc(1, sizeof(DictionaryNode));
    prev->next = newNode;
    newNode->key = key;
    newNode->list = (DictListNode*)calloc(1, sizeof(DictListNode));
    newNode->list->element = element;
    newNode->next = NULL;

    ++self->size;
}

DictListNode* dict_remove(Dictionary* self, u32 key) {
    if (!self) {
        return NULL;
    }

    u32 index = key % DICT_BUCKET_COUNT;

    DictionaryNode* node = &self->buckets[index];
    DictionaryNode* prev = NULL;

    void* result = NULL;

    while (node != NULL) {
        if (node->key == key) {
            result = node->list;
            if (prev == NULL) {
                if (node->next) {
                    self->buckets[index] = *node->next;
                } else {
                    self->buckets[index].key = DICT_INVALID_KEY;
                    self->buckets[index].list = NULL;
                }
            } else {
                prev->next = node->next;
                free(node);
            }            
            --self->size;
            break;
        }
        prev = node;
        node = node->next;
    }

    return result;
}

void dict_clear(Dictionary* self) {
    for (u32 i = 0; i < DICT_BUCKET_COUNT; ++i) {
        DictionaryNode* node = &self->buckets[i];

        while (node != NULL) {
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
            node = node->next;
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

    u32 index = key % DICT_BUCKET_COUNT;

    DictionaryNode* node = &self->buckets[index];

    while (node != NULL) {
        if (node->key == key) {
            return node->list;
        }
        node = node->next;
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
