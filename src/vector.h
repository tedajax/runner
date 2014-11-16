#ifndef _DYN_ARR_H_
#define _DYN_ARR_H_

#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "types.h"

typedef void(*free_func)(void *);

typedef struct vector_t {
	u32 size;
	u32 capacity;
	free_func freeFunc;
	void **data;
} Vector;

Vector *vector_new(u32 capacity, free_func freeFunc);
void vector_free(Vector *self);
void vector_setFreeFunc(Vector *self, free_func freeFunc);
void vector_add(Vector *self, void *element);
void *vector_index(Vector *self, u32 index);
bool vector_remove(Vector *self, void *element);
bool vector_removeAt(Vector *self, u32 index);
void vector_clear(Vector *self);

void _vector_resize(Vector *self, u32 capacity);

#endif