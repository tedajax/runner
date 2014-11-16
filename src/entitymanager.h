#ifndef RUNNER_ENTITY_MANAGER_H
#define RUNNER_ENTITY_MANAGER_H

#include "core.h"
#include "vector.h"
#include "dict.h"
#include "entity.h"
#include "component.h"

typedef struct entity_manager_t {
    Vector entities;
    Dictionary* componentsMap[COMPONENT_LAST];
    u32 lowestEId;
} EntityManager;

EntityManager* entity_manager_new();
u32 entities_gen_entity_id(EntityManager* self);
Entity* entities_create_entity(EntityManager* self);
void entities_add_component(EntityManager* self, Component* component, Entity* entity);
Component* entities_get_component(EntityManager* self, ComponentType type, Entity* entity);
void entities_remove_entity(EntityManager* self, Entity* entity);
Vector* entities_get_all_of(EntityManager* self, ComponentType type);

#endif