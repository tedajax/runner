#include "bgmanagercomponent.h"

BgManagerComponent* bg_manager_component_new(Entity entity, u32 twidth, u32 theight) {
    BgManagerComponent* self = CALLOC(1, BgManagerComponent);

    component_init((Component*)self, COMPONENT_BG_MANAGER, sizeof(BgManagerComponent), entity);

    self->tileWidth = twidth;
    self->tileHeight = theight;

    u32 x = globals.world.width / self->tileWidth + 2;
    u32 y = globals.world.height / self->tileHeight + 2;

    self->rows = y;
    self->columns = x;

    self->capacity= x * y;
    self->transforms = CALLOC(self->capacity, TransformComponent*);
    self->count = 0;

    return self;
}

COMPONENT_DESERIALIZE(COMPONENT_BG_MANAGER) {
    char* textureName = CONFIG_GET(string)(config, table, "texture");
    SDL_Texture* texture = textures_get(textureName);
    ASSERT(texture, "Failed to load texture.");
    int width, height;
    SDL_QueryTexture(texture, NULL, NULL, &width, &height);
    return (Component*)bg_manager_component_new(0, (u32)width, (u32)height);
}

bool bg_manager_component_add_entity(BgManagerComponent* self, TransformComponent* transform) {
    if (self->count >= self->capacity) {
        return false;
    }

    self->transforms[self->count] = transform;
    ++self->count;

    return true;
}