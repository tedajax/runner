#include "collisionsystem.h"
#include "primitives.h"
#include "game.h"

static bool layerMatrix[COLLIDER_LAYER_LAST][COLLIDER_LAYER_LAST];

void collision_system_init(CollisionSystem* self, EntityManager* entityManager) {
    aspect_system_init(&self->super, entityManager, COMPONENT_COLLIDER);
    SET_SYSTEM_COMPARISON(entityManager, self, collider_component_compare);

    _layer_matrix_set(COLLIDER_LAYER_PLAYER, COLLIDER_LAYER_ENEMY, true);
    _layer_matrix_set(COLLIDER_LAYER_PLAYER, COLLIDER_LAYER_TRIGGERS, true);
    _layer_matrix_set(COLLIDER_LAYER_PLAYER, COLLIDER_LAYER_ENEMY_PROJECTILE, true);
    _layer_matrix_set(COLLIDER_LAYER_PLAYER_PROJECTILE, COLLIDER_LAYER_ENEMY, true);

    self->currentId = 0;

    REGISTER_SYSTEM_HANDLER(MESSAGE_ENTITY_REMOVED,
        collision_system_on_entity_removed);
}

i32 collision_system_gen_id(CollisionSystem* self) {
    return self->currentId++;
}

void collision_system_register_collider(CollisionSystem* self, Collider* collider) {
    collider->colliderId = collision_system_gen_id(self);
}

void collision_system_start(CollisionSystem* self) {
    GET_SYSTEM_COMPONENTS(self);

    for (u32 i = 0; i < components->count; ++i) {
        ColliderComponent* collider = (ColliderComponent*)GET_SYSTEM_COMPONENT(i);
        collision_system_register_collider(self, &collider->collider);
        TransformComponent* tx = (TransformComponent*)GET_COMPONENT(GET_ENTITY(i), COMPONENT_TRANSFORM);
        collider->collider.anchor = tx;
    }
}

void collision_system_update(CollisionSystem* self) {
    GET_SYSTEM_COMPONENTS(self);

    profiler_tick("collision_update_colliders");
    for (u32 i = 0; i < components->count; ++i) {
        ColliderComponent* collider =
            (ColliderComponent*)GET_SYSTEM_COMPONENT(i);

        REQUIRED_COMPONENTS(collider);

        if (collider->collider.colliderId < 0) {
            collision_system_register_collider(self, &collider->collider);
            TransformComponent* tx = (TransformComponent*)GET_COMPONENT(GET_ENTITY(i), COMPONENT_TRANSFORM);
            collider->collider.anchor = tx;
        }

        Vec2 anchored;
        collider_anchored_center(&collider->collider, &anchored);
        physics_volume_update(collider->collider.volume, &anchored, collider->collider.anchor->rotation, &collider->collider.anchor->scale);
    }

    profiler_tock("collision_update_colliders");

    profiler_tick("collision_sort");
    component_list_insertion_sort(components);
    profiler_tock("collision_sort");

    profiler_tick("collision_comparisons");
    for (u32 i = 0; i < components->count; ++i) {
        ColliderComponent* cc1 =
            (ColliderComponent*)GET_SYSTEM_COMPONENT(i);

        Collider* c1 = &cc1->collider;

        if (!collider_on_screen(c1)) {
            continue;
        }

        f32 right = rect_right(&c1->volume->bounds) + COLLISION_RANGE_FUDGE;
        u32 j = i + 1;
        while (j < components->count) {
            ColliderComponent* cc2 =
                (ColliderComponent*)GET_SYSTEM_COMPONENT(j);

            Collider* c2 = &cc2->collider;

            if (rect_left(&c2->volume->bounds) - COLLISION_RANGE_FUDGE > right) {
                break;
            }

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
            MessageOnCollisionParams params1;
            params1.other = e2;
            MESSAGE_SET_PARAM_BLOCK(msg1, params1);

            Message msg2;
            MessageOnCollisionParams params2;
            params2.other = e1;
            MESSAGE_SET_PARAM_BLOCK(msg2, params2);

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
    profiler_tock("collision_comparisons");
}

void collision_system_render(CollisionSystem* self) {
    GET_SYSTEM_COMPONENTS(self);

    for (u32 i = 0; i < components->count; ++i) {
        ColliderComponent* collider =
            (ColliderComponent*)GET_SYSTEM_COMPONENT(i);

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
    GET_SYSTEM_COMPONENTS(self);

    for (u32 i = 0; i < components->count; ++i) {
        Entity entity = GET_ENTITY(i);

        ColliderComponent* entityCollider =
            (ColliderComponent*)GET_SYSTEM_COMPONENT(i);

        for (u32 j = 0; j < collider->collider.inContactCount; ++j) {
            if (entityCollider->collider.colliderId == collider->collider.inContact[j]) {
                collider_set_in_contact(&entityCollider->collider,
                    &collider->collider,
                    false);

                Message message;
                message.type = MESSAGE_ON_COLLISION_EXIT;
                MessageOnCollisionParams params;
                params.other = entityCollider->collider.entity;
                MESSAGE_SET_PARAM_BLOCK(message, params);

                entities_send_message(self->super.entityManager, entity, message);
            }
        }
    }

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
