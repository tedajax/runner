#ifndef RUNNER_TRANSFORM_COMPONENT_H
#define RUNNER_TRANSFORM_COMPONENT_H

#include "core.h"
#include "component.h"

typedef struct transform_component_t {
    Component super;
    Vec2 position;
    Vec2 scale;
    f32 rotation;
} TransformComponent;

TransformComponent* transform_component_new(Entity entity,
    Vec2 position,
    f32 rotation,
    Vec2 scale);

COMPONENT_DEFINE(COMPONENT_TRANSFORM);

#endif