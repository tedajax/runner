#ifndef RUNNER_CONTROLLER_COMPONENT_H
#define RUNNER_CONTROLLER_COMPONENT_H

#include "core.h"
#include "component.h"
#include "bulletsource.h"

typedef struct controller_component_t {
    Component super;
    f32 moveSpeed;
    BulletSource* bulletSources;
    u32 bulletSourceCount;
} ControllerComponent;

ControllerComponent* controller_component_new(char* config, char* section);

#endif