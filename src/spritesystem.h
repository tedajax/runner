#ifndef RUNNER_SPRITE_SYSTEM_H
#define RUNNER_SPRITE_SYSTEM_H

#include "aspectsystem.h"
#include "component.h"
#include "transformcomponent.h"
#include "spritecomponent.h"

typedef struct sprite_system_t {
    AspectSystem super;
} SpriteSystem;

SpriteSystem* sprite_system_new(EntityManager* entityManager);
void sprite_system_update(SpriteSystem* self);
void sprite_system_render(SpriteSystem* self);

#endif