#include "movementcomponent.h"

MovementComponent* movement_component_new_zero(Entity entity) {
    return movement_component_new(entity, vec2_zero(), 0.f);
}

MovementComponent* movement_component_new(Entity entity, Vec2 velocity, f32 angularVelocity) {
    MovementComponent* self = (MovementComponent*)calloc(1, sizeof(MovementComponent));

    component_init((Component*)self, COMPONENT_MOVEMENT, sizeof(MovementComponent), entity);

    self->velocity = velocity;
    self->angularVelocity = angularVelocity;
    self->constrainToCamera = false;

    return self;
}

COMPONENT_DESERIALIZE(COMPONENT_MOVEMENT) {
    Vec2 velocity = CONFIG_TRY_GET(Vec2)(config, table, "velocity", vec2_zero());
    f32 angularVelocity = CONFIG_TRY_GET(float)(config, table, "angular_velocity", 0.f);
    bool constrain = CONFIG_TRY_GET(bool)(config, table, "constrain_to_camera", false);

    MovementComponent* result = movement_component_new(0, velocity, angularVelocity);
    result->constrainToCamera = constrain;

    return (Component*)result;
}

COMPONENT_FREE(COMPONENT_MOVEMENT) {}

COMPONENT_COPY(COMPONENT_MOVEMENT) {}