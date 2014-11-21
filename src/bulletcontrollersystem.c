#include "bulletcontrollersystem.h"

BulletControllerSystem* bullet_controller_system_new(EntityManager* entityManager) {
    BulletControllerSystem* self = (BulletControllerSystem*)calloc(1,
        sizeof(BulletControllerSystem));

    aspect_system_init(&self->super, entityManager, COMPONENT_BULLET_CONTROLLER);

    return self;
}

void bullet_controller_system_update(BulletControllerSystem* self, EntityList* entities) {
    aspect_system_entities((AspectSystem*)self, entities);

    for (u32 i = 0; i < entities->size; ++i) {
        Entity entity = entities->list[i];

        TransformComponent* transform = (TransformComponent*)entities_get_component(
            self->super.entityManager,
            COMPONENT_TRANSFORM,
            &entity);

        MovementComponent* movement = (MovementComponent*)entities_get_component(
            self->super.entityManager,
            COMPONENT_MOVEMENT,
            &entity);

        BulletControllerComponent* bullet = (BulletControllerComponent*)entities_get_component(
            self->super.entityManager,
            COMPONENT_BULLET_CONTROLLER,
            &entity);

        REQUIRED_COMPONENTS(transform && movement && bullet);

        vec2_set(&movement->velocity, bullet->speed, 0.f);

        if (transform->position.x > globals.camera.position.x + globals.world.width) {
            entities_remove_entity(self->super.entityManager, &entity);
        }
    }
}