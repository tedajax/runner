#include "collisionsystem.h"
#include "primitives.h"
#include "game.h"

static bool layerMatrix[COLLIDER_LAYER_LAST][COLLIDER_LAYER_LAST];

void collision_system_init(CollisionSystem* self, EntityManager* entityManager) {
    aspect_system_init(&self->super, entityManager, COMPONENT_COLLIDER);

    _layer_matrix_set(COLLIDER_LAYER_PLAYER, COLLIDER_LAYER_ENEMY, true);
    _layer_matrix_set(COLLIDER_LAYER_PLAYER_PROJECTILE, COLLIDER_LAYER_ENEMY, true);

    self->currentId = 0;

    self->colliders.count = 0;

    REGISTER_SYSTEM_HANDLER(MESSAGE_ENTITY_REMOVED,
        collision_system_on_entity_removed);
}

i32 collision_system_gen_id(CollisionSystem* self) {
    return self->currentId++;
}

void collision_system_register_collider(CollisionSystem* self, Collider* collider) {
    collider->colliderId = collision_system_gen_id(self);
    ColliderEntry entry;
    entry.entity = collider->entity;
    entry.collider = collider;
    entry.left = rect_left(&collider->volume->bounds);
    entry.right = rect_right(&collider->volume->bounds);
    self->colliders.entries[self->colliders.count] = entry;
    ++self->colliders.count;
}

void collision_system_unregister_collider(CollisionSystem* self, i32 id) {
    for (u32 i = 0; i < self->colliders.count; ++i) {
        Collider* colliderPtr = self->colliders.entries[i].collider;
        if (colliderPtr->colliderId == id) {
            self->colliders.entries[i] = self->colliders.entries[self->colliders.count - 1];
            --self->colliders.count;
            break;
        }
    }
}

void collision_system_start(CollisionSystem* self, EntityList* entities) {
    aspect_system_entities((AspectSystem*)self, entities);

    for (u32 i = 0; i < entities->size; ++i) {
        Entity entity = entities->list[i];
        collision_system_start_single(self, entity);
    }
}

void collision_system_start_single(CollisionSystem* self, Entity entity) {
    ColliderComponent* collider =
        (ColliderComponent*)GET_COMPONENT(entity, COMPONENT_COLLIDER);

    REQUIRED_COMPONENTS(collider);

    collision_system_register_collider(self, &collider->collider);
}

void collision_system_update(CollisionSystem* self, EntityList* entities) {
    aspect_system_entities((AspectSystem*)self, entities);

    if (entities->size == 0) {
        return;
    }

    for (u32 i = 0; i < entities->size; ++i) {
        Entity entity = entities->list[i];

        ColliderComponent* collider =
            (ColliderComponent*)GET_COMPONENT(entity, COMPONENT_COLLIDER);

        REQUIRED_COMPONENTS(collider);

        if (collider->collider.colliderId < 0) {
            collision_system_register_collider(self, &collider->collider);
        }

        Vec2 anchored;
        collider_anchored_center(&collider->collider, &anchored);
        physics_volume_update(collider->collider.volume, &anchored, collider->collider.anchor->rotation, &collider->collider.anchor->scale);
    }

    for (u32 i = 0; i < self->colliders.count; ++i) {
        ColliderEntry* entry = &self->colliders.entries[i];
        entry->left = rect_left(&entry->collider->volume->bounds);
        entry->right = rect_right(&entry->collider->volume->bounds);
    }

    for (u32 i = 1; i < self->colliders.count - 1; ++i) {
        u32 j = i;
        while (j > 0 && self->colliders.entries[j - 1].left > self->colliders.entries[j].left) {
            ColliderEntry temp = self->colliders.entries[j - 1];
            self->colliders.entries[j - 1] = self->colliders.entries[j];
            self->colliders.entries[j] = temp;
            --j;
        }
    }

    for (u32 i = 0; i < self->colliders.count; ++i) {
        ColliderEntry* entry1 = &self->colliders.entries[i];
        Collider* c1 = entry1->collider;

        if (!collider_on_screen(c1)) {
            continue;
        }

        u32 j = i + 1;
        while (j < self->colliders.count && self->colliders.entries[j].left < entry1->right) {
            ColliderEntry* entry2 = &self->colliders.entries[j];
            Collider* c2 = entry2->collider;

            ++j;

            if (!collider_on_screen(c2)) {
                continue;
            }

            if (!layerMatrix[c1->layer][c2->layer]) {
                continue;
            }

            Entity e1 = c1->entity;
            Entity e2 = c2->entity;

            Message msg1;
            msg1.params[0] = &e1;

            Message msg2;
            msg2.params[0] = &e2;

            if (collider_is_colliding(c1, c2)) {
                bool inContact = collider_in_contact(c1, c2);

                if (!inContact) {
                    collider_set_in_contact(c1, c2, true);
                    msg1.type = MESSAGE_ON_COLLISION_ENTER;
                    msg2.type = MESSAGE_ON_COLLISION_ENTER;
                    entities_send_message(self->super.entityManager, e1, msg1);
                    entities_send_message(self->super.entityManager, e2, msg2);
                }
                else {
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

    //for (u32 i = 0; i < entities->size - 1; ++i) {
    //    Entity e1 = entities->list[i];

    //    ColliderComponent* cc1 =
    //        (ColliderComponent*)GET_COMPONENT(e1, COMPONENT_COLLIDER);

    //    Collider* c1 = &cc1->collider;

    //    // Generate new id for new collision systems
    //    if (c1->colliderId < 0) {
    //        c1->colliderId = collision_system_gen_id(self);
    //    }

    //    if (!collider_on_screen(c1)) {
    //        continue;
    //    }

    //    for (u32 j = i + 1; j < entities->size; ++j) {
    //        Entity e2 = entities->list[j];

    //        ColliderComponent* cc2 =
    //            (ColliderComponent*)GET_COMPONENT(e2, COMPONENT_COLLIDER);
    //        
    //        Collider* c2 = &cc2->collider;

    //        if (c2->colliderId < 0) {
    //            c2->colliderId = collision_system_gen_id(self);
    //        }

    //        if (!collider_on_screen(c2)) {
    //            continue;
    //        }

    //        if (!layerMatrix[c1->layer][c2->layer]) {
    //            continue;
    //        }

    //        Message msg1;
    //        msg1.params[0] = &e2;

    //        Message msg2;
    //        msg2.params[0] = &e1;

    //        if (collider_is_colliding(c1, c2)) {
    //            bool inContact = collider_in_contact(c1, c2);

    //            if (!inContact) {
    //                collider_set_in_contact(c1, c2, true);
    //                msg1.type = MESSAGE_ON_COLLISION_ENTER;
    //                msg2.type = MESSAGE_ON_COLLISION_ENTER;
    //                entities_send_message(self->super.entityManager, e1, msg1);
    //                entities_send_message(self->super.entityManager, e2, msg2);
    //            } else {
    //                msg1.type = MESSAGE_ON_COLLISION_STAY;
    //                msg2.type = MESSAGE_ON_COLLISION_STAY;
    //                entities_send_message(self->super.entityManager, e1, msg1);
    //                entities_send_message(self->super.entityManager, e2, msg2);
    //            }
    //        } else {
    //            bool inContact = collider_in_contact(c1, c2);

    //            if (inContact) {
    //                collider_set_in_contact(c1, c2, false);

    //                msg1.type = MESSAGE_ON_COLLISION_EXIT;
    //                msg2.type = MESSAGE_ON_COLLISION_EXIT;
    //                entities_send_message(self->super.entityManager, e1, msg1);
    //                entities_send_message(self->super.entityManager, e2, msg2);
    //            }
    //        }
    //    }
    //}
}

void collision_system_render(CollisionSystem* self, EntityList* entities) {
    aspect_system_entities((AspectSystem*)self, entities);

    for (u32 i = 0; i < entities->size; ++i) {
        Entity entity = entities->list[i];

        ColliderComponent* collider =
            (ColliderComponent*)GET_COMPONENT(entity, COMPONENT_COLLIDER);

        REQUIRED_COMPONENTS(collider);

        Rect bounds;
        rect_copy_to(&collider->collider.volume->bounds, &bounds);
        bounds.position.x -= globals.camera.position.x;
        bounds.position.y -= globals.camera.position.y;
        prim_rect_color(globals.renderer, &bounds, 0xFF00FFFF);

        u32 color = 0xFF00FF00;
        if (collider->collider.inContactCount > 0) {
            color = 0xFF0000FF;
        }

        switch (collider->collider.volume->type) {
            default:
            case BOUNDING_VOLUME_AA_BOX: {
                    Rect r;
                    AABoundingBox aabb = *((AABoundingBox*)collider->collider.volume);
                    r.position.x = aabb.center.x - aabb.width / 2.f - globals.camera.position.x;
                    r.position.y = aabb.center.y - aabb.height / 2.f - globals.camera.position.y;
                    r.width = aabb.width;
                    r.height = aabb.height;
                    prim_rect_color(globals.renderer, &r, color);
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
    EntityList* entities = &globals.game->entities;
    aspect_system_entities((AspectSystem*)self, entities);
    
    for (u32 i = 0; i < entities->size; ++i) {
        Entity entity = entities->list[i];

        ColliderComponent* entityCollider =
            (ColliderComponent*)GET_COMPONENT(entity, COMPONENT_COLLIDER);

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

    collision_system_unregister_collider(self, collider->collider.colliderId);

    free(collider->collider.volume);
}

void collision_system_on_entity_removed(AspectSystem* system, Entity entity, const Message msg) {
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
