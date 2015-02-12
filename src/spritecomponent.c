#include "spritecomponent.h"

SpriteComponent* sprite_component_new(Entity entity, Atlas* atlas, char* spriteName, i32 layer) {
    SpriteComponent* self = (SpriteComponent*)calloc(1, sizeof(SpriteComponent));

    component_init((Component*)self, COMPONENT_SPRITE, sizeof(SpriteComponent), entity);

    self->atlas = atlas;
    self->layer = layer;

    self->redTimer = 0.f;

    sprite_component_set_sprite(self, spriteName);

    return self;
}

void sprite_component_set_sprite(SpriteComponent* self, char* spriteName) {
    SpriteFrame* frame = atlas_get_frame(self->atlas, spriteName);
    ASSERT(frame, "Unable to find sprite.");
    self->currentFrame = *frame;
    self->width = (int)self->currentFrame.frame.width;
    self->height = (int)self->currentFrame.frame.height;
}

COMPONENT_DESERIALIZE(COMPONENT_SPRITE) {
    /*char* textureStr = CONFIG_GET(string)(config, table, "texture");
    i32 layer = (i32)CONFIG_TRY_GET(int)(config, table, "layer", 0);
    return (Component*)sprite_component_new(0, textures_get(textureStr), layer);*/
    return NULL;
}

COMPONENT_FREE(COMPONENT_SPRITE) {}

COMPONENT_COPY(COMPONENT_SPRITE) {}

void sprite_component_source(SpriteComponent* self, SDL_Rect* source) {
    source->x = (int)self->currentFrame.frame.position.x;
    source->y = (int)self->currentFrame.frame.position.y;
    source->w = (int)self->currentFrame.frame.width;
    source->h = (int)self->currentFrame.frame.height;
}

void sprite_component_destination(SpriteComponent* self, TransformComponent* transform, SDL_Rect* dest) {
    f32 px = transform->position.x - globals.camera.position.x + self->currentFrame.offset.x;
    f32 py = transform->position.y - globals.camera.position.y + self->currentFrame.offset.y;

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