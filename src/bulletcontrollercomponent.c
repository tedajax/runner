#include "bulletcontrollercomponent.h"

BulletControllerComponent* bullet_controller_component_new(f32 speed) {
    BulletControllerComponent* self = (BulletControllerComponent*)calloc(1,
        sizeof(BulletControllerComponent));

    self->super.type = COMPONENT_BULLET_CONTROLLER;

    self->speed = speed;

    return self;
}