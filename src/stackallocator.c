#include "stackallocator.h"

void stackor_init(StackAllocator* self, u32 bytes) {
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

void* stackor_alloc(StackAllocator* self, u32 bytes) {
    ASSERT(self->block, "Cannot allocate to uninitialized stack block.");
    ASSERT(self->head + bytes <= self->size, "Attempt to allocate beyond block bounds.");

    u32 offset = self->head;
    self->head += bytes;

    return ((u8*)self->block + offset);
}

void* stackor_get(StackAllocator* self, u32 offset) {
    ASSERT(self->block, "Cannot get from unallocated block.");
    ASSERT(offset <= self->head, "Attempt to get memory beyond allocated block.");

    return ((u8*)self->block + offset);
}

void stackor_free(StackAllocator* self, u32 bytes) {
    ASSERT(self->head >= bytes, "Attempt to free more bytes than are available from allocator.");

    self->head -= bytes;
}

void stackor_free_to(StackAllocator* self, void* location) {
    ASSERT((u32)location <= (u32)self->block + self->head, "Attempt to free to a location after the head.");

    IF_DEBUG(u32 diff = ((u32)self->block + self->head) - (u32)location;)
    ASSERT(diff <= self->head, "Attempt to free to a location beyond the allocator bounds.");

    self->head = (u32)location - (u32)self->block;
}

void stackor_clear(StackAllocator* self) {
    self->head = 0;
}