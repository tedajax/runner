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

void controller_system_init(ControllerSystem* self, EntityManager* entityManager);
void controller_system_update(ControllerSystem* self, EntityList* entities);

void controller_system_on_entity_removed(AspectSystem* system, Entity entity, Message message);

#endif