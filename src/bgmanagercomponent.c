#include "bgmanagercomponent.h"

BgManagerComponent* bg_manager_component_new(Entity entity, u32 twidth, u32 theight) {
    BgManagerComponent* self = CALLOC(1, BgManagerComponent);

    component_init((Component*)self, COMPONENT_BG_MANAGER, entity);

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

bool bg_manager_component_add_entity(BgManagerComponent* self, TransformComponent* transform) {
    if (self->count >= self->capacity) {
        return false;
    }

    self->transforms[self->count] = transform;
    ++self->count;

    return true;
}