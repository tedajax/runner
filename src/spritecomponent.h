#ifndef RUNNER_SPRITE_COMPONENT_H
#define RUNNER_SPRITE_COMPONENT_H

#include "core.h"
#include "component.h"
#include "transformcomponent.h"

#include <SDL2/SDL.h>

typedef struct sprite_component_t {
    Component super;
    SDL_Texture* texture;
    i32 layer;
    int width;
    int height;
    SDL_Rect srcRect;
    f32 redTimer;
} SpriteComponent;

SpriteComponent* sprite_component_new(Entity entity, SDL_Texture* texture, i32 layer);
COMPONENT_DESERIALIZE(COMPONENT_SPRITE);
void sprite_component_destination(SpriteComponent* self, TransformComponent* transform, SDL_Rect* dest);
void sprite_component_on_collision_enter(Component* component, Message message);
int sprite_component_layer_compare(Component* c1, Component* c2);

#endif