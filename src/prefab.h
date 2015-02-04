#ifndef RUNNER_PREFAB_H
#define RUNNER_PREFAB_H

#include "types.h"
#include "debug.h"
#include "log.h"
#include "config.h"
#include "component.h"

#include <sys/types.h>
#include <sys/stat.h>

#define PREFAB_MAX_COMPONENT_NAME_LENGTH 64

typedef char PrefabString[PREFAB_MAX_COMPONENT_NAME_LENGTH];

typedef struct prefab_t {
    Config* config;
    PrefabString* componentNames;
    u32 componentCount;
} Prefab;

void prefab_init(Prefab* self, char* filename);
void prefab_free(Prefab* self);
void prefab_reload(Prefab* self);
void prefab_instantiate(Prefab* self);
void prefab_instantiate_at(Prefab* self, Vec2 position, f32 rotation);

#endif