#include "entity.h"

Entity* entity_new(u32 id) {
    Entity* self = (Entity*)calloc(1, sizeof(Entity));

    self->id = id;

    return self;
}
