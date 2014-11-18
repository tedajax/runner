#ifndef RUNNER_GRAVITY_COMPONENT_H
#define RUNNER_GRAVITY_COMPONENT_H

#include "core.h"
#include "component.h"
#include "movementcomponent.h"

typedef struct gravity_component_t {
    Component super;
    Vec2 gravAccel;
} GravityComponent;

GravityComponent* gravity_component_new(const Vec2* acceleration);
GravityComponent* gravity_component_world();

#endif