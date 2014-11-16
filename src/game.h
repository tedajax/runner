#ifndef RUNNER_GAME_H
#define RUNNER_GAME_H

#include "core.h"
#include "entitymanager.h"
#include "component.h"
#include "healthcomponent.h"
#include "healthsystem.h"

typedef struct game_t {
    EntityManager* entityManager;
    HealthSystem* healthSystem;
    Entity* player;
} Game;

void game_init(Game* self);
void game_update(Game* self);
void game_render(Game* self);

#endif