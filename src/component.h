#ifndef RUNNER_COMPONENT_H
#define RUNNER_COMPONENT_H

#include "core.h"

typedef enum component_type_e {
    COMPONENT_TRANSFORM,
    COMPONENT_HEALTH,
    COMPONENT_SPRITE_RENDERER,

    COMPONENT_LAST,
} ComponentType;

typedef struct component_t {
    ComponentType type;
} Component;

#endif