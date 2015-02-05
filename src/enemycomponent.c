#include "enemycomponent.h"

EnemyComponent* enemy_component_new(Entity entity, EnemyType enemyType) {
    EnemyComponent* self = CALLOC(1, EnemyComponent);

    component_init((Component*)self, COMPONENT_ENEMY, sizeof(EnemyComponent), entity);

    self->enemyType = enemyType;
    self->state = 0;
    self->destroyFlag = false;

    return self;
}

COMPONENT_DESERIALIZE(COMPONENT_ENEMY) {
    char* enemyTypeStr = CONFIG_TRY_GET(string)(config, table, "enemy_type", "ENEMY_TYPE_BASIC");
    return (Component*)enemy_component_new(0, enemy_type_parse(enemyTypeStr));
}

EnemyType enemy_type_parse(const char* str) {
    if (strcmp(str, "ENEMY_TYPE_BASIC") == 0) {
        return ENEMY_TYPE_BASIC;
    } else {
        return ENEMY_TYPE_BASIC;
    }
}