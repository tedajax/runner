#ifndef RUNNER_GAME_H
#define RUNNER_GAME_H

#include "core.h"
#include "entitymanager.h"
#include "entityfactory.h"
#include "components.h"
#include "systems.h"
#include "debughud.h"

typedef struct game_t {
    EntityManager* entityManager;
    MovementSystem movementSystem;
    ControllerSystem controllerSystem;
    BulletControllerSystem bulletControllerSystem;
    GravitySystem gravitySystem;
    HealthSystem healthSystem;
    SpriteSystem spriteSystem;
    SpriteSystem backgroundSpriteSystem;
    BgManagerSystem bgManagerSystem;
    EnemySystem enemySystem;
    CollisionSystem collisionSystem;
    Entity player;

    EntityList entities;

    DebugHud debugHud;
} Game;

void game_init(Game* self);
void game_quit(Game* self);
void game_start(Game* self);
void game_update(Game* self);
void game_render(Game* self);

#endif