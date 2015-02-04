#include "hashset.h"

Hashset *hashset_new(u32 buckets) {
    Hashset *self = (Hashset *)calloc(1, sizeof(Hashset));

    hashset_init(self, buckets);

    return self;
}

void hashset_init(Hashset* self, u32 buckets) {
    self->bucketCount = buckets;
    self->buckets = (Vector **)calloc(buckets, sizeof(Vector *));

    for (u32 i = 0; i < self->bucketCount; ++i)
    {
        self->buckets[i] = NULL;
    }

    self->size = 0;
}

void hashset_add(Hashset *self, const char *key) {
    assert(self);
    assert(key);

    u64 hash = _hashset_djb2(key);
    u32 index = _hashset_index(self, key);
    Vector *bucket = NULL;

    if (!self->buckets[index]) {
        self->buckets[index] = vector_new(8, free);
    }

    bucket = self->buckets[index];

    for (u32 i = 0; i < bucket->size; ++i) {
        void *pkvp = vector_index(bucket, i);

        HashsetNode *kvp = (HashsetNode *)pkvp;

        // key already exists in hashset, set it to true and return success
        if (kvp->key == hash) {
            if (!kvp->value) {
                ++self->size;
            }
            kvp->value = true;
            return;
        }
    }

    HashsetNode *kvp = (HashsetNode *)calloc(1, sizeof(HashsetNode));
    kvp->key = hash;
    kvp->value = true;
    vector_add(bucket, kvp);
    ++self->size;
}

bool hashset_contains(Hashset *self, const char *key) {
    assert(self);
    assert(key);

    u64 hash = _hashset_djb2(key);
    u32 index = _hashset_index(self, key);
    Vector *bucket = self->buckets[index];

    if (!bucket) {
        return false;
    }

    for (u32 i = 0; i < bucket->size; ++i) {
        void *pkvp = vector_index(bucket, i);
        HashsetNode *kvp = (HashsetNode *)pkvp;
        if (kvp->key == hash) {
            return kvp->value;
        }
    }

    return false;
}

void hashset_remove(Hashset *self, const char *key) {
    assert(self);
    assert(key);

    u64 hash = _hashset_djb2(key);
    u32 index = _hashset_index(self, key);
    Vector *bucket = self->buckets[index];

    if (!bucket) {
        return;
    }

    for (u32 i = 0; i < bucket->size; ++i) {
        void *pkvp = vector_index(bucket, i);
        HashsetNode *kvp = (HashsetNode *)pkvp;
        if (kvp->key == hash) {
            if (kvp->value) {
                --self->size;
            }
            kvp->value = false;
        }
    }
}

void hashset_free(Hashset *self) {
    for (u32 i = 0; i < self->bucketCount; ++i) {
        if (self->buckets[i]) {
            vector_free(self->buckets[i]);
        }
    }

    free(self->buckets);
}

u64 _hashset_djb2(const char *key) {
    u64 hash = 5381;
    i32 c;
    while ((c = *key++)) {
        hash = ((hash << 5) + hash) + c; //hash * 33 + c
    }
    return hash;
}

u32 _hashset_index(Hashset *self, const char *key) {
    return _hashset_djb2(key) % self->bucketCount;
}