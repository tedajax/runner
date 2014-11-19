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
    f32 sclx = (f32)globals.screen.width / globals.world.width;
    f32 scly = (f32)globals.screen.height / globals.world.height;

    f32 px = transform->position.x - globals.camera.position.x;
    f32 py = transform->position.y - globals.camera.position.y;

    i16 x = (i16)(px * sclx);
    i16 y = (i16)(py * scly);
    i16 w = (i16)((f32)self->width * sclx);
    i16 h = (i16)((f32)self->height * scly);

    dest->x = x;
    dest->y = y;
    dest->w = w;
    dest->h = h;
}