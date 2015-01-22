#ifndef RUNNER_COLLISION_H
#define RUNNER_COLLISION_H

#include "aspectsystem.h"
#include "component.h"
#include "collidercomponent.h"
#include "collider.h"
#include "pool.h"

#define COLLISION_MAX_COLLIDERS (MAX_ENTITIES * 4)

#define COLLISION_RANGE_FUDGE 8.f

typedef struct collider_entry_t {
    Entity entity;
    f32 left;
    f32 right;
    Collider* collider;
} ColliderEntry;

typedef struct collider_list_t {
    ColliderEntry entries[COLLISION_MAX_COLLIDERS];
    u32 count;
} ColliderList;

typedef struct collision_system_t {
    AspectSystem super;
    i32 currentId;
    ColliderList colliders;
} CollisionSystem;

void collision_system_init(CollisionSystem* self, EntityManager* entityManager);
i32 collision_system_gen_id(CollisionSystem* self);
void collision_system_start(CollisionSystem* self, EntityList* entities);
void collision_system_start_single(CollisionSystem* self, Entity entity);
void collision_system_register_collider(CollisionSystem* self, Collider* collider);
void collision_system_unregister_collider(CollisionSystem* self, i32 id);
void collision_system_update(CollisionSystem* self, EntityList* entities);
void collision_system_render(CollisionSystem* self, EntityList* entities);
void collision_system_remove_collider(CollisionSystem* self, ColliderComponent* collider);

void collision_system_on_entity_removed(AspectSystem* system, Entity entity, const Message msg);

void _layer_matrix_set(ColliderLayer l1, ColliderLayer l2, bool value);

#endif