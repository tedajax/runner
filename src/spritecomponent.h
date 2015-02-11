#ifndef RUNNER_SPRITE_COMPONENT_H
#define RUNNER_SPRITE_COMPONENT_H

#include "core.h"
#include "component.h"
#include "transformcomponent.h"
#include "animation.h"

#include <SDL2/SDL.h>

#include <stdarg.h>

typedef struct sprite_component_t {
    Component super;
    Atlas* atlas;
    
    //TODO: need an animation controller or something
    //Hashtable animationTable;
    //Animation* currentAnimation;
    
    SpriteFrame currentFrame;
    i32 layer;
    int width;
    int height;
    SDL_Rect srcRect;
    f32 redTimer;
} SpriteComponent;

COMPONENT_DEFINE(COMPONENT_SPRITE);

SpriteComponent* sprite_component_new(Entity entity, Atlas* atlas, char* spriteName, i32 layer);
void sprite_component_set_sprite(SpriteComponent* self, char* spriteName);
void sprite_component_source(SpriteComponent* self, SDL_Rect* source);
void sprite_component_destination(SpriteComponent* self, TransformComponent* transform, SDL_Rect* dest);
void sprite_component_on_collision_enter(Component* component, Message message);
int sprite_component_layer_compare(Component* c1, Component* c2);

#endif