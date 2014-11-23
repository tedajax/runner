#ifndef RUNNER_SPRITE_COMPONENT_H
#define RUNNER_SPRITE_COMPONENT_H

#include "core.h"
#include "component.h"
#include "transformcomponent.h"

#include <SDL2/SDL.h>

typedef struct sprite_component_t {
    Component super;
    SDL_Texture* texture;
    u32 layer;
    int width;
    int height;
    SDL_Rect srcRect;
    f32 redTimer;
} SpriteComponent;

SpriteComponent* sprite_component_new(SDL_Texture* texture, u32 layer);
void sprite_component_destination(SpriteComponent* self, TransformComponent* transform, SDL_Rect* dest);
void sprite_component_on_collision_enter(Component* component, Message message);

#endif