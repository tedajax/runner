#ifndef RUNNER_ASPECT_SYSTEM_H
#define RUNNER_ASPECT_SYSTEM_H

#include "core.h"
#include "entitymanager.h"

typedef struct aspect_system_t {
    EntityManager* entityManager;
    ComponentType systemType;
} AspectSystem;

void aspect_system_init(AspectSystem* self, EntityManager* entityManager, ComponentType type);
void aspect_system_entities(AspectSystem* self, EntityList* dest);

#endif