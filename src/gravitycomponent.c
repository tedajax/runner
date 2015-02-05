#include "gravitycomponent.h"

GravityComponent* gravity_component_new(Entity entity, const Vec2* acceleration) {
    GravityComponent* self = (GravityComponent*)calloc(1, sizeof(GravityComponent));

    component_init((Component*)self, COMPONENT_GRAVITY, sizeof(GravityComponent), entity);
    vec2_set(&self->gravAccel, acceleration->x, acceleration->y);

    return self;
}

COMPONENT_DESERIALIZE(COMPONENT_GRAVITY) {
    Vec2 gravity = CONFIG_TRY_GET(Vec2)(config, table, "gravity", globals.gravity);
    return (Component*)gravity_component_new(0, &gravity);
}
