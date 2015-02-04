#include "enemycomponent.h"

EnemyComponent* enemy_component_new(Entity entity, EnemyType enemyType) {
    EnemyComponent* self = CALLOC(1, EnemyComponent);

    component_init((Component*)self, COMPONENT_ENEMY, sizeof(EnemyComponent), entity);

    self->enemyType = enemyType;
    self->state = 0;
    self->destroyFlag = false;

    return self;
}
