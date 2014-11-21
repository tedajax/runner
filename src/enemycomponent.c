#include "enemycomponent.h"

EnemyComponent* enemy_component_new(EnemyType enemyType) {
    EnemyComponent* self = CALLOC(1, EnemyComponent);

    self->super.type = COMPONENT_ENEMY;

    self->enemyType = enemyType;
    self->state = 0;

    return self;
}