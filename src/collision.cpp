#include "collision.h"

// Collider

void collider_init_circle(Collider* self, Vec2 offset, f32 radius, Entity* entity, Vec2* anchor) {
    self->colliderId = -1;
    
    self->shape = COLLIDER_SHAPE_CIRCLE;
    circle_set(&self->circle, &offset, radius);

    self->entity = entity;
    self->anchor = anchor;

    self->inContactCount = 0;
}

void collider_init_rectangle(Collider* self, Vec2 offset, f32 width, f32 height, Entity* entity, Vec2* anchor) {
    self->colliderId = -1;

    self->shape = COLLIDER_SHAPE_RECTANGLE;
    rect_set(&self->rectangle, &offset, width, height);

    self->entity = entity;
    self->anchor = anchor;

    self->inContactCount = 0;
}

void collider_copy(const Collider* source, Collider* dest) {
    dest->colliderId = source->colliderId;
    dest->shape = source->shape;
    circle_copy_to(&source->circle, &dest->circle);
    rect_copy_to(&source->rectangle, &dest->rectangle);
    dest->entity = source->entity;
    dest->anchor = source->anchor;
    dest->inContactCount = source->inContactCount;
    for (size_t i = 0; i < dest->inContactCount; ++i) {
        dest->inContact[i] = source->inContact[i];
    }
}

// Collision Manager

void collision_init(CollisionManager* self, EntityManager* entityManager) {
    self->entityManager = entityManager;

    self->currentId = 0;
    self->capacity = 256;
    self->count = 0;
    self->colliders = CALLOC(self->capacity, Collider*);
}

i32 collision_gen_id(CollisionManager* self) {
    return self->currentId++;
}

void collision_register_collider(CollisionManager* self, Collider* collider) {
    if (self->count >= self->capacity) {
        //this breakpoint is here because this is probably something we wont
        //want to support and I want to be aware of this reallocation
        DEBUG_BREAKPOINT();
        self->capacity <<= 1;
        self->colliders = (Collider**)realloc(self->colliders, self->capacity * sizeof(Collider*));
    }

    collider->colliderId = collision_gen_id(self);
    self->colliders[self->count] = collider;
    ++self->count;
}

void collision_unregister_collider(CollisionManager* self, i32 id) {
    for (size_t i = 0; i < self->count; ++i) {
        if (self->colliders[i]->colliderId == id) {
            for (size_t j = i + 1; j < self->count; ++j) {
                self->colliders[j - 1] = self->colliders[j];
            }
            --self->count;
            break;
        }
    }
}

void collision_update(CollisionManager* self) {

}