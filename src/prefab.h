#ifndef RUNNER_PREFAB_H
#define RUNNER_PREFAB_H

#include "types.h"
#include "debug.h"
#include "log.h"
#include "config.h"
#include "component.h"

#include <sys/types.h>
#include <sys/stat.h>

typedef struct prefab_component_t {
    ComponentType type;
} PrefabComponent;

typedef struct prefab_t {
    char* path;
    time_t lastMTime;
    
} Prefab;

void prefab_init(Prefab* self, char* filename);
bool prefab_update_mtime(Prefab* self);
bool prefab_check_and_reload(Prefab* self);
void prefab_instantiate(Prefab* self);
void prefab_instantiate_at(Prefab* self, Vec2 position, f32 rotation);

#endif