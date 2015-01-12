#ifndef RUNNER_BULLET_CONTROLLER_COMPONENT_H
#define RUNNER_BULLET_CONTROLLER_COMPONENT_H

#include "core.h"
#include "component.h"

typedef struct bullet_config_t {
    dynf32 speed;
    dynf32 angle;
    f32 baseAngle;
    f32 lifetime;
    i32 damage;
    char* colliderKey;
} BulletConfig;

typedef struct bullet_controller_component_t {
    Component super;
    BulletConfig config;
    dynf32 speed;
    dynf32 angle;
    f32 baseAngle;
    f32 lifeTimer;
    bool destroy;
} BulletControllerComponent;

BulletControllerComponent* bullet_controller_component_new(BulletConfig* config);
void bullet_controller_on_collision_enter(Component* component, Message message);

#endif