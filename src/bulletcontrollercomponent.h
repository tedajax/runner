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
} BulletConfig;

typedef struct bullet_controller_component_t {
    Component super;
    BulletConfig config;
    dynf32 speed;
    dynf32 angle;
    f32 baseAngle;
    f32 lifeTimer;
    bool destroyFlag;
} BulletControllerComponent;

BulletControllerComponent* bullet_controller_component_new(Entity entity, BulletConfig* config);
COMPONENT_DEFINE(COMPONENT_BULLET_CONTROLLER);

#endif