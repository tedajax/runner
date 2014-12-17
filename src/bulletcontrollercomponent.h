#ifndef RUNNER_BULLET_CONTROLLER_COMPONENT_H
#define RUNNER_BULLET_CONTROLLER_COMPONENT_H

#include "core.h"
#include "component.h"

typedef struct bullet_config_t {
    f32 startAngle;
    f32 startSpeed;
    f32 acceleration;
    f32 rotationRate;
    f32 lifetime;
} BulletConfig;

typedef struct bullet_controller_component_t {
    Component super;
    BulletConfig config;
    f32 speed;
    f32 angle;
    f32 lifeTimer;
    bool destroy;
} BulletControllerComponent;

BulletControllerComponent* bullet_controller_component_new(BulletConfig* config);
void bullet_controller_on_collision_enter(Component* component, Message message);

#endif