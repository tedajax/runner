#include "enemysystem.h"

void enemy_system_init(EnemySystem* self, EntityManager* entityManager) {
    aspect_system_init(&self->super, entityManager, COMPONENT_ENEMY, 64);
}

void enemy_system_update(EnemySystem* self) {
    GET_SYSTEM_COMPONENTS(self);

    TransformComponent* playerTransform =
        (TransformComponent*)entities_get_component(self->super.entityManager,
        COMPONENT_TRANSFORM,
        globals.player);

    for (u32 i = 0; i < components->count; ++i) {
        EnemyComponent* enemy = (EnemyComponent*)GET_SYSTEM_COMPONENT(i);
        enemy_update(self, enemy, playerTransform);
    }
}

void enemy_update(EnemySystem* self, EnemyComponent* enemy, TransformComponent* playerTransform) {
    Entity entity = enemy->super.entity;

    TransformComponent* transform =
        (TransformComponent*)GET_COMPONENT(entity, COMPONENT_TRANSFORM);

    MovementComponent* movement =
        (MovementComponent*)GET_COMPONENT(entity, COMPONENT_MOVEMENT);

    REQUIRED_COMPONENTS(enemy, transform, movement);
    
    switch (enemy->enemyType) {
        default:
        case ENEMY_TYPE_LAST:
            return;

        case ENEMY_TYPE_BASIC:
            switch (enemy->state) {
                case 0:
                    if (transform->position.x < playerTransform->position.x + 1000.f) {
                        enemy->state = 1;
                        movement->velocity.x = -500.f;
                    }
                    break;
            }

            if (transform->position.x < globals.levelPosition - 256.f) {
                enemy->destroyFlag = true;
            }
            break;
    }

    if (enemy->destroyFlag) {
        entities_remove_entity(self->super.entityManager, entity);
    }
}