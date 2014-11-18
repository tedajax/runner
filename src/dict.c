#include "dict.h"

const u32 DICT_INVALID_KEY = 0xFFFFFFFF;

Dictionary* dict_new(size_t buckets) {
    Dictionary* self = (Dictionary*)calloc(1, sizeof(Dictionary));

    self->bucketCount = buckets;
    self->buckets = (DictionaryNode*)calloc(buckets, sizeof(DictionaryNode));
    for (u32 i = 0; i < self->bucketCount; ++i) {
        self->buckets[i].key = DICT_INVALID_KEY;
        self->buckets[i].list = NULL;
        self->buckets[i].next = NULL;
    }
    self->size = 0;

    return self;
}

void dict_init(Dictionary* self, size_t buckets) {
    self->bucketCount = buckets;
    self->buckets = (DictionaryNode*)calloc(buckets, sizeof(DictionaryNode));
    for (u32 i = 0; i < self->bucketCount; ++i) {
        self->buckets[i].key = DICT_INVALID_KEY;
        self->buckets[i].list = NULL;
        self->buckets[i].next = NULL;
    }
    self->size = 0;
}

void dict_set(Dictionary* self, u32 key, void* element) {
    if (!self) {
        return;
    }

    if (key == DICT_INVALID_KEY) {
        return;
    }

    size_t index = key % self->bucketCount;

    DictionaryNode* prev = NULL;
    DictionaryNode* node = &self->buckets[index];

    if (node->key == DICT_INVALID_KEY) {
        DictionaryNode* newNode = (DictionaryNode*)calloc(1, sizeof(DictionaryNode));
        self->buckets[index] = *newNode;
        newNode->key = key;
        newNode->list = (DictListNode*)calloc(1, sizeof(DictListNode));
        newNode->next = NULL;

        ++self->size;
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

    size_t index = key % self->bucketCount;

    DictionaryNode* node = &self->buckets[index];
    DictionaryNode* prev = NULL;

    void* result = NULL;

    while (node != NULL) {
        if (node->key == key) {
            result = node->list;
            if (prev == NULL) {
                self->buckets[index] = *node->next;
            } else {
                prev->next = node->next;
            }

            --self->size;
        }
        prev = node;
        node = node->next;
    }

    return result;
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

    size_t index = key % self->bucketCount;

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
