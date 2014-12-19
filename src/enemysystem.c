#include "enemysystem.h"

void enemy_system_init(EnemySystem* self, EntityManager* entityManager) {
    aspect_system_init(&self->super, entityManager, COMPONENT_ENEMY);
}

void enemy_system_update(EnemySystem* self, EntityList* entities) {
    aspect_system_entities((AspectSystem*)self, entities);

    TransformComponent* playerTransform =
        (TransformComponent*)entities_get_component(self->super.entityManager,
        COMPONENT_TRANSFORM,
        globals.player);

    for (u32 i = 0; i < entities->size; ++i) {
        Entity entity = entities->list[i];

        enemy_update(self, &entity, playerTransform);
    }
}

void enemy_update(EnemySystem* self, Entity* entity, TransformComponent* playerTransform) {
    EnemyComponent* enemy =
        (EnemyComponent*)GET_COMPONENT(*entity, COMPONENT_ENEMY);

    TransformComponent* transform =
        (TransformComponent*)GET_COMPONENT(*entity, COMPONENT_TRANSFORM);

    MovementComponent* movement =
        (MovementComponent*)GET_COMPONENT(*entity, COMPONENT_MOVEMENT);

    REQUIRED_COMPONENTS(enemy && transform && movement);

    if (input_key(SDL_SCANCODE_J)) {
        transform->position.x -= 100.f * globals.time.delta;
    }
    if (input_key(SDL_SCANCODE_L)) {
        transform->position.x += 100.f * globals.time.delta;
    }
    if (input_key(SDL_SCANCODE_I)) {
        transform->position.y -= 100.f * globals.time.delta;
    }
    if (input_key(SDL_SCANCODE_K)) {
        transform->position.y += 100.f * globals.time.delta;
    }

    /*switch (enemy->enemyType) {
        default:
        case ENEMY_TYPE_INVALID:
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
            break;
    }*/
}