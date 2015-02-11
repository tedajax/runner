#include "spritecomponent.h"

SpriteComponent* sprite_component_new(Entity entity, Atlas* atlas, i32 layer, ...) {
    va_list args;
    va_start(layer, args);
    
    SpriteComponent* result = sprite_component_newv(entity, atlas, layer, args);

    va_end(args);

    return result;
}

SpriteComponent* sprite_component_newv(Entity entity, Atlas* atlas, i32 layer, va_list args) {
    SpriteComponent* self = (SpriteComponent*)calloc(1, sizeof(SpriteComponent));

    component_init((Component*)self, COMPONENT_SPRITE, sizeof(SpriteComponent), entity);

    animation_init()

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

COMPONENT_DESERIALIZE(COMPONENT_SPRITE) {
    char* textureStr = CONFIG_GET(string)(config, table, "texture");
    i32 layer = (i32)CONFIG_TRY_GET(int)(config, table, "layer", 0);
    return (Component*)sprite_component_new(0, textures_get(textureStr), layer);
}

COMPONENT_FREE(COMPONENT_SPRITE) {}

COMPONENT_COPY(COMPONENT_SPRITE) {}

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