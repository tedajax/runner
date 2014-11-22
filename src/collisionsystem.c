#include "collisionsystem.h"

void collision_system_init(CollisionSystem* self, EntityManager* entityManager) {
    aspect_system_init(&self->super, entityManager, COMPONENT_COLLIDER);

    self->currentId = 0;
    self->count = 0;
    memset(self->colliders, 0, sizeof(Collider*) * COLLISION_MAX_COLLIDERS);
}

i32 collision_system_gen_id(CollisionSystem* self) {
    return self->currentId++;
}

void collision_system_register_collider(CollisionSystem* self, Collider* collider) {
    ASSERT(self->count < COLLISION_MAX_COLLIDERS, "Reach collider limit.");
    collider->colliderId = collision_system_gen_id(self);
    self->colliders[self->count] = collider;
    ++self->count;
}

void collision_system_unregister_collider(CollisionSystem* self, i32 id) {
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

void collision_system_update(CollisionSystem* self, EntityList* entities) {
    aspect_system_entities((AspectSystem*)self, entities);

    if (entities->size == 0) {
        return;
    }

    for (size_t i = 0; i < entities->size - 1; ++i) {
        for (size_t j = i + 1; j < entities->size; ++j) {
            Entity* e1 = &entities->list[i];
            Entity* e2 = &entities->list[j];

            ColliderComponent* cc1 =
                (ColliderComponent*)GET_COMPONENT(*e1, COMPONENT_COLLIDER);

            ColliderComponent* cc2 =
                (ColliderComponent*)GET_COMPONENT(*e2, COMPONENT_COLLIDER);

            Collider c1 = cc1->collider;
            Collider c2 = cc2->collider;

            if (collider_is_colliding(&c1, &c2)) {
                printf("collision bitches\n");
            }
        }
    }
}