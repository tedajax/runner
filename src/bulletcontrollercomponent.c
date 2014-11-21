#include "bulletcontrollercomponent.h"

BulletControllerComponent* bullet_controller_component_new(f32 speed) {
    BulletControllerComponent* self = (BulletControllerComponent*)calloc(1,
        sizeof(BulletControllerComponent));

    component_init((Component*)self, COMPONENT_BULLET_CONTROLLER);

    self->speed = speed;

    return self;
}