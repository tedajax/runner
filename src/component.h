#ifndef RUNNER_COMPONENT_H
#define RUNNER_COMPONENT_H

#include "core.h"
#include "messaging.h"
#include "hashset.h"

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

#define COMPONENT_REGISTER(type) hashtable_insert(&COMPONENT_NAME_TABLE, #type, &COMPONENT_VALUE_TABLE[type]);

typedef struct component_t {
    ComponentType type;
    Entity entity;
} Component;

void component_system_init();
void component_system_terminate();

void component_init(Component* self, ComponentType type, Entity entity);
void component_set_entity(Component* self, Entity entity);
void component_free(Component* self);
void component_free_void(void* self);

int component_entity_compare(Component* c1, Component* c2);

ComponentType component_parse_type(char* str);
Component* component_deserialize(Config* config, const char* table);
#endif