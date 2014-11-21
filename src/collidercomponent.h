#ifndef RUNNER_COLLIDER_COMPONENT_H
#define RUNNER_COLLIDER_COMPONENT_H

#include "core.h"
#include "component.h"

typedef enum collider_shape_e {
    COLLIDER_SHAPE_INVALID,
    COLLIDER_SHAPE_CIRCLE,
    COLLIDER_SHAPE_RECTANGLE,
    COLLIDER_SHAPE_LAST,
} ColliderShape;

typedef struct collider_component_t {
    Component super;
    ColliderShape shape;
    i32 colliderId;
    Circle circle;
    Rect rectangle;
} ColliderComponent;

ColliderComponent* collider_component_new_circle(Vec2* offset, f32 radius);
ColliderComponent* collider_component_new_rectangle(Vec2* offset, f32 width, f32 height);

#endif