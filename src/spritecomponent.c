#include "spritecomponent.h"

SpriteComponent* sprite_component_new(SDL_Texture* texture) {
    SpriteComponent* self = (SpriteComponent*)calloc(1, sizeof(SpriteComponent));

    self->super.type = COMPONENT_SPRITE;
    self->texture = texture;

    SDL_QueryTexture(self->texture,
        NULL,
        NULL,
        &self->width,
        &self->height);

    //TODO: pull actual values from texture

    return self;
}

void sprite_component_destination(SpriteComponent* self, TransformComponent* transform, SDL_Rect* dest) {
    dest->x = (i16)transform->position.x;
    dest->y = (i16)transform->position.y;
    dest->w = self->width;
    dest->h = self->height;
}