#ifndef RUNNER_COLLISION_H
#define RUNNER_COLLISION_H

#include "aspectsystem.h"
#include "component.h"
#include "collidercomponent.h"
#include "collider.h"
#include "pool.h"

#define COLLISION_MAX_COLLIDERS (MAX_ENTITIES * 4)

#define COLLISION_RANGE_FUDGE 8.f

typedef struct collision_system_t {
    AspectSystem super;
    i32 currentId;
} CollisionSystem;

void collision_system_init(CollisionSystem* self, EntityManager* entityManager);
i32 collision_system_gen_id(CollisionSystem* self);
void collision_system_start(CollisionSystem* self);
void collision_system_start_single(CollisionSystem* self, Entity entity);
void collision_system_register_collider(CollisionSystem* self, Collider* collider);
void collision_system_update(CollisionSystem* self);
void collision_system_render(CollisionSystem* self);
void collision_system_remove_collider(CollisionSystem* self, ColliderComponent* collider);

void collision_system_on_entity_removed(AspectSystem* system, Entity entity, const Message msg);

void _layer_matrix_set(ColliderLayer l1, ColliderLayer l2, bool value);

#endif