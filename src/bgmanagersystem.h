#ifndef RUNNER_BG_MANAGER_SYSTEM_H
#define RUNNER_BG_MANAGER_SYSTEM_H

#include "aspectsystem.h"
#include "component.h"
#include "transformcomponent.h"
#include "bgmanagercomponent.h"

typedef struct bg_manager_system_t {
    AspectSystem super;
} BgManagerSystem;

void bg_manager_system_init(BgManagerSystem* self, EntityManager* entityManager);
void bg_manager_system_start(BgManagerSystem* self, EntityList* entities);
void bg_manager_system_update(BgManagerSystem* self, EntityList* entities);

#endif