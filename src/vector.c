#include "vector.h"

Vector *vector_new(u32 capacity, free_func freeFunc) {
	Vector *self = (Vector *)malloc(sizeof(Vector));

	self->size = 0;
	self->freeFunc = freeFunc;
	self->data = NULL;
	_vector_resize(self, capacity);

	return self;
}

void vector_init(Vector* self, u32 capacity, free_func freeFunc) {
	self->size = 0;
	self->freeFunc = freeFunc;
	self->data = NULL;
	_vector_resize(self, capacity);
}

void vector_free(Vector *self) {
	assert(self);

	if (self->freeFunc) {
		for (u32 i = 0; i < self->size; ++i) {
			void *element = self->data[i];
			self->freeFunc(element);
		}
	}

	self->freeFunc = NULL;

	free(self->data);
	free(self);
}

void vector_setFreeFunc(Vector *self, free_func freeFunc) {
	assert(self);
	self->freeFunc = freeFunc;
}

void vector_add(Vector *self, void *element) {
	assert(self);
	assert(element);

	if (self->size >= self->capacity) {
		_vector_resize(self, self->capacity * 2);
	}

	self->data[self->size] = element;
	++self->size;
}

void *vector_index(Vector *self, u32 index) {
	assert(self);

	if (index >= self->size) {
		return NULL;
	}

	return self->data[index];
}

bool vector_remove(Vector *self, void *element) {
	assert(self);
	assert(element);

	for (u32 i = 0; i < self->size; ++i) {
		if (self->data[i] == element) {
			return vector_removeAt(self, i);;
		}
	}

	return false;
}

bool vector_removeAt(Vector *self, u32 index) {
	assert(self);

	if (index >= self->size) {
		return false;
	}

	if (self->freeFunc != NULL) {
		self->freeFunc(self->data[index]);
	} else {
		free(self->data[index]);
	}

	u32 i = index;
	while (i + 1 < self->size) {
		self->data[i] = self->data[i + 1];
		++i;
	}

	--self->size;

	return true;
}

void vector_clear(Vector *self) {
	assert(self);

	for (u32 i = 0; i < self->size; ++i) {
		if (self->freeFunc) {
			self->freeFunc(self->data[i]);
		}
	}

	self->size = 0;
}

void _vector_resize(Vector *self, u32 capacity) {
	assert(self);

	if (capacity <= self->size) {
		capacity = self->size + 1;
	}

	self->capacity = capacity;
	self->data = realloc(self->data, sizeof(void *) * capacity);
}

