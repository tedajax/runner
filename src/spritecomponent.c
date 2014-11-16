#include "spritecomponent.h"

SpriteComponent* sprite_component_new(SDL_Texture* texture) {
    SpriteComponent* self = (SpriteComponent*)calloc(1, sizeof(SpriteComponent));

    self->super.type = COMPONENT_SPRITE;
    self->texture = texture;

    //TODO: pull actual values from texture

    return self;
}