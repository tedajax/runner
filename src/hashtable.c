#include "hashtable.h"

Hashtable *hashtable_new(u32 buckets, hashtable_free_f freeFunc) {
    Hashtable *self = (Hashtable *)calloc(1, sizeof(Hashtable));

    self->bucketCount = buckets;
    self->buckets = (Vector **)calloc(buckets, sizeof(Vector *));

    for (u32 i = 0; i < self->bucketCount; ++i) {
        self->buckets[i] = NULL;
    }

    self->freeFunc = freeFunc;

    return self;
}

void hashtable_init(Hashtable* self, u32 buckets, hashtable_free_f freeFunc) {
    self->bucketCount = buckets;
    self->buckets = (Vector **)calloc(buckets, sizeof(Vector *));

    for (u32 i = 0; i < self->bucketCount; ++i) {
        self->buckets[i] = NULL;
    }

    self->freeFunc = freeFunc;
}

bool hashtable_insert(Hashtable *self, const char *key, void *data) {
    assert(self);
    assert(key);
    assert(data);

    u64 hash = _hashtable_djb2(key);
    u32 index = _hashtable_index(self, key);
    Vector *bucket = NULL;

    if (!self->buckets[index]) {
        self->buckets[index] = vector_new(8, self->freeFunc);
    }

    bucket = self->buckets[index];

    //search for the key in the bucket and if we find it return false
    for (u32 i = 0; i < bucket->size; ++i) {
        void *pkvp = vector_index(bucket, i);

        HashtableNode *kvp = (HashtableNode *)pkvp;
        if (kvp->key == hash) {
            return false;
        }
    }

    //since we have ruled out key collisions we can now safely add to the table
    //wrap the data up in a key value pair and insert to the bucket
    HashtableNode *kvp = (HashtableNode *)malloc(sizeof(HashtableNode));
    kvp->key = hash;
    kvp->value = data;
    vector_add(bucket, kvp);

    return true;
}

void *hashtable_get(Hashtable *self, const char *key) {
    assert(self);
    assert(key);

    u64 hash = _hashtable_djb2(key);
    u32 index = _hashtable_index(self, key);
    Vector *bucket = self->buckets[index];

    if (!bucket) {
        return NULL;
    }

    for (u32 i = 0; i < bucket->size; ++i) {
        void *pkvp = vector_index(bucket, i);
        HashtableNode *kvp = (HashtableNode *)pkvp;
        if (kvp->key == hash) {
            return kvp->value;
        }
    }

    return NULL;
}

void *hashtable_remove(Hashtable *self, const char *key) {
    assert(self);
    assert(key);

    u64 hash = _hashtable_djb2(key);
    u32 index = _hashtable_index(self, key);
    Vector *bucket = self->buckets[index];

    if (!bucket) {
        return NULL;
    }

    for (u32 i = 0; i < bucket->size; ++i) {
        void *pkvp = vector_index(bucket, i);
        HashtableNode *kvp = (HashtableNode *)pkvp;
        if (kvp->key == hash) {
            void *presult = kvp->value;
            vector_removeAt(bucket, i);
            return presult;
        }
    }

    return NULL;
}

void hashtable_clear(Hashtable* self) {
    for (u32 i = 0; i < self->bucketCount; ++i) {
        Vector *bucket = self->buckets[i];
        if (bucket) {
            for (u32 j = 0; j < bucket->size; ++j) {
                HashtableNode *kvp = (HashtableNode *)vector_index(bucket, j);
                if (self->freeFunc) {
                    self->freeFunc(kvp->value);
                }
                free(kvp);
            }

            vector_clear(bucket);
        }
    }
}

void hashtable_free(Hashtable *self) {
    assert(self);

    for (u32 i = 0; i < self->bucketCount; ++i) {
        Vector *bucket = self->buckets[i];
        if (bucket) {
            for (u32 j = 0; j < bucket->size; ++j) {
                HashtableNode *kvp = (HashtableNode *)vector_index(bucket, j);
                if (self->freeFunc) {
                    self->freeFunc(kvp->value);
                }
                free(kvp);
            }

            vector_free(bucket);
        }
    }

    free(self->buckets);
    free(self);
}

u64 _hashtable_djb2(const char *key) {
    u64 hash = 5381;
    i32 c;
    while ((c = *key++)) {
        hash = ((hash << 5) + hash) + c; //hash * 33 + c
    }
    return hash;
}

u32 _hashtable_index(Hashtable *self, const char *key) {
    return _hashtable_djb2(key) % self->bucketCount;
}