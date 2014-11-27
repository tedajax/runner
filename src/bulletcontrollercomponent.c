#include "bulletcontrollercomponent.h"

BulletControllerComponent* bullet_controller_component_new(f32 speed) {
    BulletControllerComponent* self = (BulletControllerComponent*)calloc(1,
        sizeof(BulletControllerComponent));

    component_init((Component*)self, COMPONENT_BULLET_CONTROLLER);

    self->speed = speed;
    self->destroy = false;

    return self;
}

void bullet_controller_on_collision_enter(Component* component, Message message) {
    BulletControllerComponent* self = (BulletControllerComponent*)component;

    self->destroy = true;
}