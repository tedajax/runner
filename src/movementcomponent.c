#include "movementcomponent.h"

MovementComponent* movement_component_new_zero(Entity entity) {
    return movement_component_new(entity, vec2_zero(), 0.f);
}

MovementComponent* movement_component_new(Entity entity, Vec2 velocity, f32 angularVelocity) {
    MovementComponent* self = (MovementComponent*)calloc(1, sizeof(MovementComponent));

    component_init((Component*)self, COMPONENT_MOVEMENT, entity);

    self->velocity = velocity;
    self->angularVelocity = angularVelocity;
    self->constrainToCamera = false;

    return self;
}