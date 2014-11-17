#ifndef RUNNER_COMPONENT_H
#define RUNNER_COMPONENT_H

#include "core.h"

typedef enum component_type_e {
    COMPONENT_INVALID,

    COMPONENT_TRANSFORM,
    COMPONENT_MOVEMENT,

    COMPONENT_CONTROLLER,
    
    COMPONENT_HEALTH,
    COMPONENT_SPRITE,

    COMPONENT_LAST,
} ComponentType;

typedef struct component_t {
    ComponentType type;
} Component;

#endif