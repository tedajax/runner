#ifndef RUNNER_MOVEMENT_SYSTEM_H
#define RUNNER_MOVEMENT_SYSTEM_H

#include "aspectsystem.h"
#include "component.h"
#include "transformcomponent.h"
#include "movementcomponent.h"

typedef struct movement_system_t {
    AspectSystem super;
} MovementSystem;

void movement_system_init(MovementSystem* self, EntityManager* entityManager);
void movement_system_update(MovementSystem* self, EntityList* entities);

#endif