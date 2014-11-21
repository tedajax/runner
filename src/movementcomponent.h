#ifndef RUNNER_MOVEMENT_COMPONENT_H
#define RUNNER_MOVEMENT_COMPONENT_H

#include "core.h"
#include "component.h"

typedef struct movement_component_t {
    Component super;
    Vec2 velocity;
    f32 angularVelocity;
    bool constrainToCamera;
} MovementComponent;

MovementComponent* movement_component_new_zero();
MovementComponent* movement_component_new(Vec2 velocity, f32 angularVelocity);

#endif