#ifndef RUNNER_ENEMY_SYSTEM_H
#define RUNNER_ENEMY_SYSTEM_H

#include "aspectsystem.h"
#include "component.h"
#include "transformcomponent.h"
#include "movementcomponent.h"
#include "enemycomponent.h"

typedef struct enemy_system_t {
    AspectSystem super;
} EnemySystem;

void enemy_system_init(EnemySystem* self, EntityManager* entityManager);
void enemy_system_update(EnemySystem* self, EntityList* entities);
void enemy_update(EnemySystem* self, Entity entity, TransformComponent* playerTransform);

#endif