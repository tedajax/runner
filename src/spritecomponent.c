#include "spritecomponent.h"

SpriteComponent* sprite_component_new(Entity entity, SDL_Texture* texture, i32 layer) {
    SpriteComponent* self = (SpriteComponent*)calloc(1, sizeof(SpriteComponent));

    component_init((Component*)self, COMPONENT_SPRITE, entity);

    self->texture = texture;

    self->layer = layer;

    SDL_QueryTexture(self->texture,
        NULL,
        NULL,
        &self->width,
        &self->height);

    //TODO: pull actual values from texture

    self->redTimer = 0.f;

    return self;
}

void sprite_component_destination(SpriteComponent* self, TransformComponent* transform, SDL_Rect* dest) {
    f32 px = transform->position.x - globals.camera.position.x;
    f32 py = transform->position.y - globals.camera.position.y;

    int x = (int)px;
    int y = (int)py;
    int w = (int)self->width;
    int h = (int)self->height;

    dest->x = x;
    dest->y = y;
    dest->w = w;
    dest->h = h;
}

void sprite_component_on_collision_enter(Component* component, Message message) {
    SpriteComponent* self = (SpriteComponent*)component;
    
    self->redTimer = 0.05f;
}

int sprite_component_layer_compare(Component* c1, Component* c2) {
    return ((SpriteComponent*)c1)->layer - ((SpriteComponent*)c2)->layer;
}