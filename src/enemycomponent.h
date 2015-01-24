#ifndef RUNNER_ENEMY_COMPONENT_H
#define RUNNER_ENEMY_COMPONENT_H

#include "core.h"
#include "component.h"

typedef enum enemy_type_e {
    ENEMY_TYPE_INVALID,

    ENEMY_TYPE_BASIC,

    ENEMY_TYPE_LAST,
} EnemyType;

typedef struct enemy_component_t {
    Component super;
    EnemyType enemyType;
    i32 state;
} EnemyComponent;

EnemyComponent* enemy_component_new(Entity entity, EnemyType enemyType);

#endif