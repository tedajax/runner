#include "dict.h"

Dictionary* dict_new(size_t buckets) {
    Dictionary* self = (Dictionary*)calloc(1, sizeof(Dictionary));

    self->bucketCount = buckets;
    self->buckets = (DictionaryNode*)calloc(buckets, sizeof(DictionaryNode));
    self->size = 0;

    return self;
}

void dict_init(Dictionary* self, size_t buckets) {
    self->bucketCount = buckets;
    self->buckets = (DictionaryNode*)calloc(buckets, sizeof(DictionaryNode));
    self->size = 0;
}

void dict_set(Dictionary* self, u32 key, void* element) {
    if (!self) {
        return;
    }

    size_t index = key % self->bucketCount;

    DictionaryNode* prev = NULL;
    DictionaryNode* node = &self->buckets[index];

    if (node == NULL) {
        DictionaryNode* newNode = (DictionaryNode*)calloc(1, sizeof(DictionaryNode));
        self->buckets[index] = *newNode;
        newNode->key = key;
        newNode->element = element;
        newNode->next = NULL;

        ++self->size;
    }

    while (node != NULL) {
        if (node->key == key) {
            node->element = element;
            return;
        }
        prev = node;
        node = node->next;
    }

    DictionaryNode* newNode = (DictionaryNode*)calloc(1, sizeof(DictionaryNode));
    prev->next = newNode;
    newNode->key = key;
    newNode->element = element;
    newNode->next = NULL;

    ++self->size;
}

void* dict_remove(Dictionary* self, u32 key) {
    if (!self) {
        return NULL;
    }

    size_t index = key % self->bucketCount;

    DictionaryNode* node = &self->buckets[index];
    DictionaryNode* prev = NULL;

    void* result = NULL;

    while (node != NULL) {
        if (node->key == key) {
            result = node->element;
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

void* dict_get(Dictionary* self, u32 key) {
    if (!self) {
        return NULL;
    }

    size_t index = key % self->bucketCount;

    DictionaryNode* node = &self->buckets[index];

    while (node != NULL) {
        if (node->key == key) {
            return node->element;
        }
        node = node->next;
    }

    return NULL;
}