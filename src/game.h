#ifndef RUNNER_GAME_H
#define RUNNER_GAME_H

#include "core.h"
#include "entitymanager.h"
#include "component.h"
#include "healthcomponent.h"
#include "healthsystem.h"
#include "spritecomponent.h"
#include "spritesystem.h"
#include "movementcomponent.h"
#include "movementsystem.h"

typedef struct game_t {
    EntityManager* entityManager;
    HealthSystem* healthSystem;
    SpriteSystem* spriteSystem;
    MovementSystem* movementSystem;
    Entity* player;
} Game;

void game_init(Game* self);
void game_update(Game* self);
void game_render(Game* self);

#endif