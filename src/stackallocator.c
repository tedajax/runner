#include "stackallocator.h"

void stackor_init(StackAllocator* self, size_t bytes) {
    self->block = calloc(bytes, sizeof(char));

    ASSERT(self->block, "Failed to allocate block.");

    self->size = bytes;
    self->head = 0;
}

void stackor_delete(StackAllocator* self) {
    ASSERT(self->head == 0, "Attempt to delete uncleared stack allocator.");

    free(self->block);
    self->block = NULL;
}

void* stackor_alloc(StackAllocator* self, size_t bytes) {
    ASSERT(self->block, "Cannot allocate to uninitialized stack block.");
    ASSERT(self->head + bytes <= self->size, "Attempt to allocate beyond block bounds.");

    size_t offset = self->head;
    self->head += bytes;

    return ((u8*)self->block + offset);
}

void* stackor_get(StackAllocator* self, size_t offset) {
    ASSERT(self->block, "Cannot get from unallocated block.");
    ASSERT(offset <= self->head, "Attempt to get memory beyond allocated block.");

    return ((u8*)self->block + offset);
}

void stackor_free(StackAllocator* self, size_t bytes) {
    ASSERT(self->head >= bytes, "Attempt to free more bytes than are available from allocator.");

    self->head -= bytes;
}

void stackor_free_to(StackAllocator* self, void* location) {
    ASSERT((size_t)location <= (size_t)self->block + self->head, "Attempt to free to a location after the head.");

    IF_DEBUG(size_t diff = ((size_t)self->block + self->head) - (size_t)location;)
    ASSERT(diff <= self->head, "Attempt to free to a location beyond the allocator bounds.");

    self->head = (size_t)location - (size_t)self->block;
}

void stackor_clear(StackAllocator* self) {
    self->head = 0;
}