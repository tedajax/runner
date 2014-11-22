#ifndef RUNNER_COLLISION_H
#define RUNNER_COLLISION_H

#include "core.h"
#include "entitymanager.h"

#define COLLISION_MAX_COLLISIONS 32

typedef enum collider_shape_e {
    COLLIDER_SHAPE_INVALID,
    COLLIDER_SHAPE_CIRCLE,
    COLLIDER_SHAPE_RECTANGLE,
    COLLIDER_SHAPE_LAST,
} ColliderShape;

typedef struct collider_t {
    i32 colliderId;
    ColliderShape shape;
    Circle circle;
    Rect rectangle;
    Entity* entity;
    Vec2* anchor;
    i32 inContact[COLLISION_MAX_COLLISIONS];
    size_t inContactCount;
} Collider;

void collider_init_circle(Collider* self, Vec2 offset, f32 radius, Entity* entity, Vec2* anchor);
void collider_init_rectangle(Collider* self, Vec2 offset, f32 width, f32 height, Entity* entity, Vec2* anchor);
void collider_copy(const Collider* source, Collider* dest);

typedef struct collision_manager_t {
    EntityManager* entityManager;
    i32 currentId;
    size_t capacity;
    size_t count;
    Collider* colliders;
} CollisionManager;

void collision_init(CollisionManager* self, EntityManager* entityManager);
i32 collision_gen_id(CollisionManager* self);
void collision_register_collider(CollisionManager* self, Collider* collider);
void collision_unregister_collider(CollisionManager* self, i32 id);
void collision_update(CollisionManager* self);

#endif