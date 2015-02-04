#include "gravitycomponent.h"

GravityComponent* gravity_component_new(Entity entity, const Vec2* acceleration) {
    GravityComponent* self = (GravityComponent*)calloc(1, sizeof(GravityComponent));

    component_init((Component*)self, COMPONENT_GRAVITY, sizeof(GravityComponent), entity);
    vec2_set(&self->gravAccel, acceleration->x, acceleration->y);

    return self;
}

GravityComponent* gravity_component_world(Entity entity) {
    return gravity_component_new(entity, &globals.gravity);
}
