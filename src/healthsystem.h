#ifndef RUNNER_HEALTH_SYSTEM_H
#define RUNNER_HEALTH_SYSTEM_H

#include "aspectsystem.h"
#include "healthcomponent.h"

typedef struct health_system_t {
    AspectSystem super;
} HealthSystem;

HealthSystem* health_system_new(EntityManager* entityManager);
void health_system_update(HealthSystem* self);
void health_system_render(HealthSystem* self);

#endif