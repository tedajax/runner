#ifndef RUNNER_SPRITE_SYSTEM_H
#define RUNNER_SPRITE_SYSTEM_H

#include "aspectsystem.h"
#include "component.h"
#include "transformcomponent.h"
#include "spritecomponent.h"

typedef struct sprite_system_t {
    AspectSystem super;
    u32 layer;
} SpriteSystem;

void sprite_system_init(SpriteSystem* self, EntityManager* entityManager, u32 layer);
void sprite_system_update(SpriteSystem* self, EntityList* entities);
void sprite_system_render(SpriteSystem* self, EntityList* entities);

#endif