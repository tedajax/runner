#ifndef RUNNER_CONTROLLER_SYSTEM_H
#define RUNNER_CONTROLLER_SYSTEM_H

#include "aspectsystem.h"
#include "component.h"
#include "transformcomponent.h"
#include "movementcomponent.h"
#include "controllercomponent.h"

typedef struct controller_system_t {
    AspectSystem super;
} ControllerSystem;

ControllerSystem* controller_system_new(EntityManager* entityManager);
void controller_system_update(ControllerSystem* self);

#endif