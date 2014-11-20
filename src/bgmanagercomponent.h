#ifndef RUNNER_BG_MANAGER_COMPONENT
#define RUNNER_BG_MANAGER_COMPONENT

#include "core.h"
#include "component.h"
#include "entity.h"
#include "transformcomponent.h"
#include "entitymanager.h"

typedef struct bg_manager_component_t {
    Component super;
    u32 rows;
    u32 columns;
    u32 tileWidth;
    u32 tileHeight;
    TransformComponent** transforms;
    size_t capacity;
    size_t count;
} BgManagerComponent;

BgManagerComponent* bg_manager_component_new(u32 twidth, u32 theight);
bool bg_manager_component_add_entity(BgManagerComponent* self, TransformComponent* transform);

#endif