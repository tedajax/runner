#include "collisionsystem.h"

static bool layerMatrix[COLLIDER_LAYER_LAST][COLLIDER_LAYER_LAST];

void collision_system_init(CollisionSystem* self, EntityManager* entityManager) {
    aspect_system_init(&self->super, entityManager, COMPONENT_COLLIDER);

    _layer_matrix_set(COLLIDER_LAYER_PLAYER, COLLIDER_LAYER_ENEMY, true);
    _layer_matrix_set(COLLIDER_LAYER_PLAYER_PROJECTILE, COLLIDER_LAYER_ENEMY, true);

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

void collision_system_start(CollisionSystem* self, EntityList* entities) {
    aspect_system_entities((AspectSystem*)self, entities);

    for (size_t i = 0; i < entities->size; ++i) {
        Entity entity = entities->list[i];
        collision_system_start_single(self, &entity);
    }
}

void collision_system_start_single(CollisionSystem* self, Entity* entity) {
    ColliderComponent* collider =
        (ColliderComponent*)GET_COMPONENT(*entity, COMPONENT_COLLIDER);

    REQUIRED_COMPONENTS(collider);

    collider->collider.colliderId = collision_system_gen_id(self);
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

            Collider* c1 = &cc1->collider;
            Collider* c2 = &cc2->collider;

            // Generate new id for new collision systems
            if (c1->colliderId < 0) {
                c1->colliderId = collision_system_gen_id(self);
            }

            if (c2->colliderId < 0) {
                c2->colliderId = collision_system_gen_id(self);
            }

            if (!layerMatrix[c1->layer][c2->layer]) {
                continue;
            }

            if (collider_is_colliding(c1, c2)) {
                bool inContact = collider_in_contact(c1, c2);

                Message message;
                message.params[0] = e1;
                message.params[1] = e2;

                if (!inContact) {
                    collider_set_in_contact(c1, c2, true);
                    message.type = MESSAGE_ON_COLLISION_ENTER;
                    entities_send_message(self->super.entityManager, e1, message);
                    entities_send_message(self->super.entityManager, e2, message);
                } else {
                    message.type = MESSAGE_ON_COLLISION_STAY;
                    entities_send_message(self->super.entityManager, e1, message);
                    entities_send_message(self->super.entityManager, e2, message);
                }
            } else {
                bool inContact = collider_in_contact(c1, c2);

                if (inContact) {
                    collider_set_in_contact(c1, c2, false);

                    Message message;
                    message.type = MESSAGE_ON_COLLISION_EXIT;
                    message.params[0] = e1;
                    message.params[1] = e2;
                    entities_send_message(self->super.entityManager, e1, message);
                    entities_send_message(self->super.entityManager, e2, message);
                }
            }
        }
    }
}

void collision_system_remove_collider(CollisionSystem* self, ColliderComponent* collider) {
    EntityList entities;
    entities.capacity = 256;
    entities.list = (Entity*)calloc(entities.capacity, sizeof(Entity));
    aspect_system_entities((AspectSystem*)self, &entities);
    
    for (size_t i = 0; i < entities.size; ++i) {
        Entity* entity = &entities.list[i];

        ColliderComponent* entityCollider =
            (ColliderComponent*)GET_COMPONENT(*entity, COMPONENT_COLLIDER);

        for (size_t j = 0; j < collider->collider.inContactCount; ++j) {
            if (entityCollider->collider.colliderId == collider->collider.inContact[j]) {
                collider_set_in_contact(&entityCollider->collider,
                    &collider->collider,
                    false);

                Message message;
                message.type = MESSAGE_ON_COLLISION_EXIT;
                message.params[0] = collider;
                message.params[1] = entityCollider;

                entities_send_message(self->super.entityManager, entity, message);
            }
        }
    }

    free(entities.list);
}

void _layer_matrix_set(ColliderLayer l1, ColliderLayer l2, bool value) {
    layerMatrix[l1][l2] = value;
    layerMatrix[l2][l1] = value;
}