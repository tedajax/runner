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

typedef Component*(*component_deserialize_f)(Config*, const char*);

#define COMPONENT_DESERIALIZE_FUNC(type) type##_deserialize
#define COMPONENT_DESERIALIZE(type) Component* COMPONENT_DESERIALIZE_FUNC(type)(Config* config, const char* table)

typedef struct component_t {
    ComponentType type;
    u64 size;
    Entity entity;
} Component;

void component_system_init();
void component_system_terminate();

void component_init(Component* self, ComponentType type, u64 size, Entity entity);
void component_copy(const Component* source, Component* dest);
void component_set_entity(Component* self, Entity entity);
void component_free(Component* self);
void component_free_void(void* self);

int component_entity_compare(Component* c1, Component* c2);

typedef struct component_size_pair_t {
    Component* component;
    u64 size;
} ComponentSizePair;

ComponentType component_parse_type(char* str);
Component* component_deserialize(Config* config, const char* table);

// Collection of created components ready to be assigned an entity id and added into the entity manager
typedef struct component_batch_t {
    Component** components;
    u32 count;
    u32 capacity;
} ComponentBatch;

void component_batch_init(ComponentBatch* self, u32 capacity);
void component_batch_zero(ComponentBatch* self);
void component_batch_add(ComponentBatch* self, Component* component);
#endif