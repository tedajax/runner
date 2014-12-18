#include "collisionsystem.h"
#include "primitives.h"

static bool layerMatrix[COLLIDER_LAYER_LAST][COLLIDER_LAYER_LAST];

void collision_system_init(CollisionSystem* self, EntityManager* entityManager) {
    aspect_system_init(&self->super, entityManager, COMPONENT_COLLIDER);

    _layer_matrix_set(COLLIDER_LAYER_PLAYER, COLLIDER_LAYER_ENEMY, true);
    _layer_matrix_set(COLLIDER_LAYER_PLAYER_PROJECTILE, COLLIDER_LAYER_ENEMY, true);

    self->currentId = 0;
    self->count = 0;
    memset(self->colliders, 0, sizeof(Collider*) * COLLISION_MAX_COLLIDERS);

    REGISTER_SYSTEM_HANDLER(MESSAGE_ENTITY_REMOVED,
        collision_system_on_entity_removed);
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
    for (u32 i = 0; i < self->count; ++i) {
        if (self->colliders[i]->colliderId == id) {
            for (u32 j = i + 1; j < self->count; ++j) {
                self->colliders[j - 1] = self->colliders[j];
            }
            --self->count;
            break;
        }
    }
}

void collision_system_start(CollisionSystem* self, EntityList* entities) {
    aspect_system_entities((AspectSystem*)self, entities);

    for (u32 i = 0; i < entities->size; ++i) {
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

    for (u32 i = 0; i < entities->size - 1; ++i) {
        for (u32 j = i + 1; j < entities->size; ++j) {
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

            Message msg1;
            msg1.params[0] = e2;

            Message msg2;
            msg2.params[0] = e1;

            if (collider_is_colliding(c1, c2)) {
                bool inContact = collider_in_contact(c1, c2);

                if (!inContact) {
                    collider_set_in_contact(c1, c2, true);
                    msg1.type = MESSAGE_ON_COLLISION_ENTER;
                    msg2.type = MESSAGE_ON_COLLISION_ENTER;
                    entities_send_message(self->super.entityManager, e1, msg1);
                    entities_send_message(self->super.entityManager, e2, msg2);
                } else {
                    msg1.type = MESSAGE_ON_COLLISION_STAY;
                    msg2.type = MESSAGE_ON_COLLISION_STAY;
                    entities_send_message(self->super.entityManager, e1, msg1);
                    entities_send_message(self->super.entityManager, e2, msg2);
                }
            } else {
                bool inContact = collider_in_contact(c1, c2);

                if (inContact) {
                    collider_set_in_contact(c1, c2, false);

                    msg1.type = MESSAGE_ON_COLLISION_EXIT;
                    msg2.type = MESSAGE_ON_COLLISION_EXIT;
                    entities_send_message(self->super.entityManager, e1, msg1);
                    entities_send_message(self->super.entityManager, e2, msg2);
                }
            }
        }
    }
}

void collision_system_render(CollisionSystem* self, EntityList* entities) {
    aspect_system_entities((AspectSystem*)self, entities);

    for (u32 i = 0; i < entities->size; ++i) {
        Entity entity = entities->list[i];

        ColliderComponent* collider =
            (ColliderComponent*)GET_COMPONENT(entity, COMPONENT_COLLIDER);

        REQUIRED_COMPONENTS(collider);

        u32 color = 0xFF00FF00;
        if (collider->collider.inContactCount > 0) {
            color = 0xFF0000FF;
        }

        switch (collider->collider.volume->type) {
            case BOUNDING_VOLUME_AA_BOX: {
                    Rect r;
                    AABoundingBox aabb = *((AABoundingBox*)collider->collider.volume);
                    r.position.x = aabb.center.x - aabb.width / 2.f - globals.camera.position.x;
                    r.position.y = aabb.center.y - aabb.height / 2.f - globals.camera.position.y;
                    r.width = aabb.width;
                    r.height = aabb.height;
                    prim_box_color(globals.renderer, &r, color);
                }
                break;

            case BOUNDING_VOLUME_O_BOX: {
                    Rect r;
                    OBoundingBox obb = *((OBoundingBox*)collider->collider.volume);
                    r.position.x = obb.center.x - obb.width / 2.f - globals.camera.position.x;
                    r.position.y = obb.center.y - obb.height / 2.f - globals.camera.position.y;
                    r.width = obb.width;
                    r.height = obb.height;
                    prim_rect_oriented_color(globals.renderer,
                        &r,
                        obb.orientation * DEG_TO_RAD,
                        color);
                }
                break;

            case BOUNDING_VOLUME_CIRCLE:
                break;
        }
    }
}

void collision_system_remove_collider(CollisionSystem* self, ColliderComponent* collider) {
    EntityList entities;
    entities.capacity = 256;
    entities.list = (Entity*)calloc(entities.capacity, sizeof(Entity));
    aspect_system_entities((AspectSystem*)self, &entities);
    
    for (u32 i = 0; i < entities.size; ++i) {
        Entity* entity = &entities.list[i];

        ColliderComponent* entityCollider =
            (ColliderComponent*)GET_COMPONENT(*entity, COMPONENT_COLLIDER);

        for (u32 j = 0; j < collider->collider.inContactCount; ++j) {
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

void collision_system_on_entity_removed(AspectSystem* system, Entity* entity, const Message msg) {
    CollisionSystem* self = (CollisionSystem*)system;

    ColliderComponent* collider = 
        (ColliderComponent*)entities_get_component(self->super.entityManager,
        COMPONENT_COLLIDER,
        entity);

    if (collider) {
        collision_system_remove_collider(self, collider);
    }
}

void _layer_matrix_set(ColliderLayer l1, ColliderLayer l2, bool value) {
    layerMatrix[l1][l2] = value;
    layerMatrix[l2][l1] = value;
}