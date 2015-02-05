#ifndef RUNNER_PREFAB_H
#define RUNNER_PREFAB_H

#include "types.h"
#include "debug.h"
#include "log.h"
#include "config.h"
#include "component.h"
#include "entitymanager.h"

void prefab_system_init(EntityManager* entityManager, const char* prefabRoot);
void prefab_system_terminate();

typedef struct prefab_t {
    Config* config;
    ComponentBatch components;
} Prefab;

void prefab_init(Prefab* self, char* filename);
void prefab_free(Prefab* self);
void prefab_reload(Prefab* self);
void prefab_instantiate(Prefab* self);
void prefab_instantiate_at(Prefab* self, Vec2 position, f32 rotation);

#endif