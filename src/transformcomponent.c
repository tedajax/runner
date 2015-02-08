#include "transformcomponent.h"

TransformComponent* transform_component_new(Entity entity,
    Vec2 position,
    f32 rotation,
    Vec2 scale) {
    TransformComponent* self = (TransformComponent*)calloc(1, sizeof(TransformComponent));

    component_init((Component*)self, COMPONENT_TRANSFORM, sizeof(TransformComponent), entity);

    self->position = position;
    self->rotation = rotation;
    self->scale = scale;

    return self;
}

COMPONENT_DESERIALIZE(COMPONENT_TRANSFORM) {
    Vec2 position = CONFIG_TRY_GET(Vec2)(config, table, "position", vec2_zero());
    f32 rotation = CONFIG_TRY_GET(float)(config, table, "rotation", 0.f);
    Vec2 scale = CONFIG_TRY_GET(Vec2)(config, table, "scale", vec2_one());
    return (Component*)transform_component_new(0, position, rotation, scale);
}

COMPONENT_FREE(COMPONENT_TRANSFORM) {}

COMPONENT_COPY(COMPONENT_TRANSFORM) {}