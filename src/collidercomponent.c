#include "collidercomponent.h"

ColliderComponent* collider_component_new_circle(Vec2* offset, f32 radius) {
    ColliderComponent* self = CALLOC(1, ColliderComponent);

    component_init((Component*)self, COMPONENT_COLLIDER);

    self->shape = COLLIDER_SHAPE_CIRCLE;
    circle_set(&self->circle, offset, radius);

    return self;
}

ColliderComponent* collider_component_new_rectangle(Vec2* offset, f32 width, f32 height) {
    ColliderComponent* self = CALLOC(1, ColliderComponent);

    component_init((Component*)self, COMPONENT_COLLIDER);

    self->shape = COLLIDER_SHAPE_RECTANGLE;
    rect_set(&self->rectangle, offset, width, height);

    return self;
}