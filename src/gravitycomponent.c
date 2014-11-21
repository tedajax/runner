#include "gravitycomponent.h"

GravityComponent* gravity_component_new(const Vec2* acceleration) {
    GravityComponent* self = (GravityComponent*)calloc(1, sizeof(GravityComponent));

    component_init((Component*)self, COMPONENT_GRAVITY);
    vec2_set(&self->gravAccel, acceleration->x, acceleration->y);

    return self;
}

GravityComponent* gravity_component_world() {
    return gravity_component_new(&globals.gravity);
}
