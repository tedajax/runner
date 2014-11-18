#ifndef RUNNER_ENTITY_H
#define RUNNER_ENTITY_H

#include "core.h"

typedef struct transform_component_t TransformComponent;

typedef struct entity_t {
    u32 id;
} Entity;

Entity* entity_new(u32 id);

#endif