#include "spritecomponent.h"

SpriteComponent* sprite_component_new(SDL_Texture* texture, u32 layer) {
    SpriteComponent* self = (SpriteComponent*)calloc(1, sizeof(SpriteComponent));

    component_init((Component*)self, COMPONENT_SPRITE);

    REGISTER_HANDLER(MESSAGE_ON_COLLISION_ENTER, sprite_component_on_collision_enter);
    REGISTER_HANDLER(MESSAGE_ON_COLLISION_EXIT, sprite_component_on_collision_exit);

    self->texture = texture;

    self->layer = layer;

    SDL_QueryTexture(self->texture,
        NULL,
        NULL,
        &self->width,
        &self->height);

    //TODO: pull actual values from texture

    self->redTimer = 0.f;
    self->greenTimer = 0.f;

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

void sprite_component_on_collision_enter(Component* component, Message message) {
    SpriteComponent* self = (SpriteComponent*)component;
    
    self->redTimer = 0.05f;
}

void sprite_component_on_collision_exit(Component* component, Message message) {
    SpriteComponent* self = (SpriteComponent*)component;

    self->greenTimer = 0.1f;
}