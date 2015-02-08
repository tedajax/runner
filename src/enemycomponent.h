#ifndef RUNNER_ENEMY_COMPONENT_H
#define RUNNER_ENEMY_COMPONENT_H

#include "core.h"
#include "component.h"

typedef enum enemy_type_e {
    ENEMY_TYPE_BASIC,

    ENEMY_TYPE_LAST,
} EnemyType;

typedef struct enemy_component_t {
    Component super;
    EnemyType enemyType;
    i32 state;
    bool destroyFlag;
} EnemyComponent;

EnemyComponent* enemy_component_new(Entity entity, EnemyType enemyType);
COMPONENT_DEFINE(COMPONENT_ENEMY);

EnemyType enemy_type_parse(const char* str);

#endif