#ifndef RUNNER_CONTROLLER_COMPONENT_H
#define RUNNER_CONTROLLER_COMPONENT_H

#include "core.h"
#include "component.h"

typedef struct controller_component_t {
    Component super;
    f32 moveSpeed;
    f32 fireDelay;
    f32 fireTimer;
} ControllerComponent;

ControllerComponent* controller_component_new(f32 moveSpeed);

#endif