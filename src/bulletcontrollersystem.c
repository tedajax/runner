#include "bulletcontrollersystem.h"

void bullet_controller_system_init(BulletControllerSystem* self, EntityManager* entityManager) {
    aspect_system_init(&self->super, entityManager, COMPONENT_BULLET_CONTROLLER);

    REGISTER_SYSTEM_HANDLER(MESSAGE_ON_COLLISION_ENTER, bullet_controller_system_on_collision_enter);
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

        bullet->speed += bullet->config.acceleration * globals.time.delta;
        bullet->angle += bullet->config.rotationRate * globals.time.delta;

        transform->rotation = bullet->angle;

        f32 bulletRadAngle = bullet->angle * DEG_TO_RAD;
        vec2_set(&movement->velocity, bullet->speed * cosf(bulletRadAngle),
                                      bullet->speed * sinf(bulletRadAngle));

        bullet->lifeTimer -= globals.time.delta;

        if (bullet->lifeTimer <= 0) {
            bullet->destroy = true;
        }

        if (transform->position.x > globals.camera.position.x + globals.world.width) {
            bullet->destroy = true;
        }

        if (bullet->destroy) {
            entities_remove_entity(self->super.entityManager, &entity);
        }
    }
}

void bullet_controller_system_on_collision_enter(AspectSystem* system, Entity* entity, Message message) {
    BulletControllerComponent* bullet =
        (BulletControllerComponent*)entities_get_component(system->entityManager,
        COMPONENT_BULLET_CONTROLLER,
        entity);

    REQUIRED_COMPONENTS(bullet);

    bullet->destroy = true;

    i32 damage = 30;

    Message damageMsg;
    message_init(&damageMsg, MESSAGE_DAMAGE);
    message_add_param(&damageMsg, &damage);

    entities_send_message(system->entityManager, (Entity*)message.params[0], damageMsg);
}