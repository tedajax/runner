#include "transformcomponent.h"

TransformComponent* transform_component_new(Vec2 position,
    f32 rotation,
    Vec2 scale) {
    TransformComponent* self = (TransformComponent*)calloc(1, sizeof(TransformComponent));

    self->position = position;
    self->rotation = rotation;
    self->scale = scale;

    return self;
}