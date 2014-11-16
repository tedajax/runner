#ifndef RUNNER_ENTITY_MANAGER_H
#define RUNNER_ENTITY_MANAGER_H

#define MAX_ENTITIES 1024

#include "core.h"
#include "vector.h"
#include "entity.h"
#include "component.h"

typedef struct entity_manager_t {
    Vector entities;
    Vector componentsByType[COMPONENT_LAST];
} EntityManager;

i32 entities_gen_entity_id(EntityManager* self);
Entity* entities_create_entity();
void entities_add_component(EntityManager* self, Component* component, Entity* entity);
Component* entities_get_component(EntityManager* self, ComponentType type, Entity* entity);
void entities_remove_entity(EntityManager* self, Entity* entity);
Vector* entities_get_all_of(EntityManager* self, ComponentType type);

#endif