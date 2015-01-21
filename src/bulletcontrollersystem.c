#include "bulletcontrollersystem.h"
#include "collidercomponent.h"
#include "physics.h"

void bullet_controller_system_init(BulletControllerSystem* self, EntityManager* entityManager)
{
    aspect_system_init(&self->super, entityManager, COMPONENT_BULLET_CONTROLLER);

    REGISTER_SYSTEM_HANDLER(MESSAGE_ON_COLLISION_ENTER, bullet_controller_system_on_collision_enter);
    REGISTER_SYSTEM_HANDLER(MESSAGE_ENTITY_REMOVED, bullet_controller_system_on_entity_removed);
}

void bullet_controller_system_update(BulletControllerSystem* self, EntityList* entities)
{
    aspect_system_entities((AspectSystem*)self, entities);

    for (u32 i = 0; i < entities->size; ++i) {
        Entity entity = entities->list[i];

        TransformComponent* transform = (TransformComponent*)entities_get_component(
            self->super.entityManager,
            COMPONENT_TRANSFORM,
            entity);

        MovementComponent* movement = (MovementComponent*)entities_get_component(
            self->super.entityManager,
            COMPONENT_MOVEMENT,
            entity);

        BulletControllerComponent* bullet = (BulletControllerComponent*)entities_get_component(
            self->super.entityManager,
            COMPONENT_BULLET_CONTROLLER,
            entity);

        REQUIRED_COMPONENTS(transform && movement && bullet);

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

void bullet_controller_system_on_collision_enter(AspectSystem* system, Entity entity, Message message)
{
    BulletControllerComponent* bullet =
        (BulletControllerComponent*)entities_get_component(system->entityManager,
        COMPONENT_BULLET_CONTROLLER,
        entity);

    REQUIRED_COMPONENTS(bullet);

    bullet->destroy = true;

    i32 damage = bullet->config.damage;

    Message damageMsg;
    message_init(&damageMsg, MESSAGE_DAMAGE);
    message_add_param(&damageMsg, &damage);

    MessageOnCollisionParams* params = (MessageOnCollisionParams*)message.paramBlock;
    Entity target = params->other;

    entities_send_message(system->entityManager, target, damageMsg);
}

void bullet_controller_system_on_entity_removed(AspectSystem* system, Entity entity, Message message)
{
    BulletControllerComponent* bullet =
        (BulletControllerComponent*)entities_get_component(system->entityManager,
        COMPONENT_BULLET_CONTROLLER,
        entity);

    if (bullet) {
        dynf32_release(&bullet->speed);
        dynf32_release(&bullet->angle);
    }
}