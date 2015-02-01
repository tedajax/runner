#ifndef RUNNER_COMPONENT_H
#define RUNNER_COMPONENT_H

#include "core.h"
#include "messaging.h"

typedef enum component_type_e {
    COMPONENT_INVALID,

    COMPONENT_TRANSFORM,
    COMPONENT_MOVEMENT,

    COMPONENT_CONTROLLER,
    COMPONENT_BULLET_CONTROLLER,
    COMPONENT_GRAVITY,

    COMPONENT_HEALTH,
    COMPONENT_SPRITE,

    COMPONENT_BG_MANAGER,

    COMPONENT_ENEMY,

    COMPONENT_COLLIDER,

    COMPONENT_LUA,

    COMPONENT_LAST,
} ComponentType;

typedef struct component_t {
    ComponentType type;
    Entity entity;
} Component;

void component_init(Component* self, ComponentType type, Entity entity);
void component_free(Component* self);
void component_free_void(void* self);

int component_entity_compare(Component* c1, Component* c2);
#endif