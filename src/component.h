#ifndef RUNNER_COMPONENT_H
#define RUNNER_COMPONENT_H

#include "core.h"

typedef enum component_types_e {
    COMPONENT_TRANSFORM,
    COMPONENT_HEALTH,
    COMPONENT_SPRITE_RENDERER,

    COMPONENT_LAST,
} ComponentTypes;

typedef struct component_t {
    ComponentTypes type;
} Component;

#endif