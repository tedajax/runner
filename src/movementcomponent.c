#include "movementcomponent.h"

MovementComponent* movement_component_new_zero() {
    return movement_component_new(vec2_zero(), 0.f);
}

MovementComponent* movement_component_new(Vec2 velocity, f32 angularVelocity) {
    MovementComponent* self = (MovementComponent*)calloc(1, sizeof(MovementComponent));

    self->super.type = COMPONENT_MOVEMENT;

    self->velocity = velocity;
    self->angularVelocity = angularVelocity;

    return self;
}