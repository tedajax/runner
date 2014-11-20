#include "spritecomponent.h"

SpriteComponent* sprite_component_new(SDL_Texture* texture, u32 layer) {
    SpriteComponent* self = (SpriteComponent*)calloc(1, sizeof(SpriteComponent));

    self->super.type = COMPONENT_SPRITE;
    self->texture = texture;

    self->layer = layer;

    SDL_QueryTexture(self->texture,
        NULL,
        NULL,
        &self->width,
        &self->height);

    //TODO: pull actual values from texture

    return self;
}

void sprite_component_destination(SpriteComponent* self, TransformComponent* transform, SDL_Rect* dest) {
    f32 px = transform->position.x - globals.camera.position.x;
    f32 py = transform->position.y - globals.camera.position.y;

    i16 x = (i16)px;
    i16 y = (i16)py;
    i16 w = (i16)self->width;
    i16 h = (i16)self->height;

    dest->x = x;
    dest->y = y;
    dest->w = w;
    dest->h = h;
}