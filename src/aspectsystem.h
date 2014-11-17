#ifndef RUNNER_ASPECT_SYSTEM_H
#define RUNNER_ASPECT_SYSTEM_H

#include "core.h"
#include "entitymanager.h"

#define GET_ENTITY_LIST()                                       \
    EntityList entities;                                        \
    entity_list_init(&entities, 64);                            \
    aspect_system_entities((AspectSystem*)self, &entities);

#define GET_COMPONENT(type, comptype, entity)   \
    (type*)entities_get_component(              \
        self->super.entityManager,              \
        comptype,                               \
        &entity);                               \

typedef struct aspect_system_t {
    EntityManager* entityManager;
    ComponentType systemType;
} AspectSystem;

void aspect_system_init(AspectSystem* self, EntityManager* entityManager, ComponentType type);
void aspect_system_entities(AspectSystem* self, EntityList* dest);

#endif