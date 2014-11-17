#include "controllercomponent.h"

ControllerComponent* controller_component_new(f32 moveSpeed) {
    ControllerComponent* self = (ControllerComponent*)calloc(1, sizeof(ControllerComponent));

    self->super.type = COMPONENT_CONTROLLER;

    self->moveSpeed = moveSpeed;

    return self;
}