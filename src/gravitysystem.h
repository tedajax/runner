#ifndef RUNNER_GRAVITY_SYSTEM_H
#define RUNNER_GRAVITY_SYSTEM_H

#include "aspectsystem.h"
#include "component.h"
#include "movementcomponent.h"
#include "gravitycomponent.h"

typedef struct gravity_system_t {
    AspectSystem super;
} GravitySystem;

GravitySystem* gravity_system_new(EntityManager* entityManager);
void gravity_system_update(GravitySystem* self);

#endif