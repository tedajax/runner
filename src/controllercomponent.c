#include "controllercomponent.h"

ControllerComponent* controller_component_new(f32 moveSpeed) {
    ControllerComponent* self = (ControllerComponent*)calloc(1, sizeof(ControllerComponent));

    self->super.type = COMPONENT_CONTROLLER;

    self->moveSpeed = moveSpeed;
    self->fireDelay = 0.05f;
    self->fireTimer = 0.f;

    return self;
}