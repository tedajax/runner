#include "bulletcontrollersystem.h"
#include "collidercomponent.h"
#include "physics.h"

void bullet_controller_system_init(BulletControllerSystem* self, EntityManager* entityManager) {
    aspect_system_init(&self->super, entityManager, COMPONENT_BULLET_CONTROLLER);

    REGISTER_SYSTEM_HANDLER(MESSAGE_ON_COLLISION_ENTER, bullet_controller_system_on_collision_enter);
    REGISTER_SYSTEM_HANDLER(MESSAGE_ENTITY_REMOVED, bullet_controller_system_on_entity_removed);
}

void bullet_controller_system_update(BulletControllerSystem* self) {
    GET_SYSTEM_COMPONENTS(self);

    for (u32 i = 0; i < components->count; ++i) {
        Entity entity = GET_ENTITY(i);
        BulletControllerComponent* bullet = (BulletControllerComponent*)GET_SYSTEM_COMPONENT(i);

        TransformComponent* transform = (TransformComponent*)GET_COMPONENT(entity, COMPONENT_TRANSFORM);

        MovementComponent* movement = (MovementComponent*)GET_COMPONENT(entity, COMPONENT_MOVEMENT);

        REQUIRED_COMPONENTS(transform, movement, bullet);

        transform->position.x += globals.scrollSpeed * globals.time.delta;

        f32 speed = dynf32_get(&bullet->speed);
        f32 angle = bullet->baseAngle + dynf32_get(&bullet->angle);

        transform->rotation = angle;

        f32 bulletRadAngle = angle * DEG_TO_RAD;
        vec2_set(&movement->velocity, speed * cosf(bulletRadAngle),
            speed * sinf(bulletRadAngle));

        bullet->lifeTimer -= globals.time.delta;

        if (bullet->lifeTimer <= 0) {
            bullet->destroy = true;
        }

        if (transform->position.x > globals.camera.position.x + globals.world.width) {
            bullet->destroy = true;
        }

        if (bullet->destroy) {
            entities_remove_entity(self->super.entityManager, entity);
        }
    }
}

void bullet_controller_system_on_collision_enter(AspectSystem* system, Entity entity, Message message) {
    BulletControllerComponent* bullet =
        (BulletControllerComponent*)entities_get_component(system->entityManager,
        COMPONENT_BULLET_CONTROLLER,
        entity);

    REQUIRED_COMPONENTS(bullet);

    bullet->destroy = true;

    i32 damage = bullet->config.damage;

    Message damageMsg;
    MessageOnDamageParams damageParams;
    damageParams.damage = damage;
    message_init(&damageMsg, MESSAGE_DAMAGE);
    MESSAGE_SET_PARAM_BLOCK(damageMsg, damageParams);

    MessageOnCollisionParams* params = (MessageOnCollisionParams*)message.paramBlock;
    Entity target = params->other;

    entities_send_message(system->entityManager, target, damageMsg);
}

void bullet_controller_system_on_entity_removed(AspectSystem* system, Entity entity, Message message) {
    BulletControllerComponent* bullet =
        (BulletControllerComponent*)entities_get_component(system->entityManager,
        COMPONENT_BULLET_CONTROLLER,
        entity);

    if (bullet) {
        dynf32_release(&bullet->speed);
        dynf32_release(&bullet->angle);
    }
}