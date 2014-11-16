#ifndef RUNNER_COMPONENT_H
#define RUNNER_COMPONENT_H

#include "core.h"

typedef enum component_type_e {
    COMPONENT_TRANSFORM,
    COMPONENT_HEALTH,
    COMPONENT_SPRITE,

    COMPONENT_LAST,
} ComponentType;

typedef struct component_t {
    ComponentType type;
} Component;

void component_init(Component* self, ComponentType type);

#endif