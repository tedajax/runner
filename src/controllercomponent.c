#include "controllercomponent.h"

ControllerComponent* controller_component_new(f32 moveSpeed) {
    ControllerComponent* self = (ControllerComponent*)calloc(1, sizeof(ControllerComponent));

    component_init((Component*)self, COMPONENT_CONTROLLER);

    self->moveSpeed = moveSpeed;
    self->fireDelay = 0.1f;
    self->fireTimer = 0.f;

    return self;
}