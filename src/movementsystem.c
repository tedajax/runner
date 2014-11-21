#include "movementsystem.h"

MovementSystem* movement_system_new(EntityManager* entityManager) {
    MovementSystem* self = (MovementSystem*)calloc(1, sizeof(MovementSystem));

    aspect_system_init(&self->super, entityManager, COMPONENT_MOVEMENT);

    return self;
}

void movement_system_update(MovementSystem* self, EntityList* entities) {
    aspect_system_entities((AspectSystem*)self, entities);

    for (u32 i = 0; i < entities->size; ++i) {
        Entity entity = entities->list[i];

        TransformComponent* transform =
            (TransformComponent*)GET_COMPONENT(entity, COMPONENT_TRANSFORM);

        MovementComponent* movement =
            (MovementComponent*)GET_COMPONENT(entity, COMPONENT_MOVEMENT);

        REQUIRED_COMPONENTS(transform && movement);

        Vec2 timeVel;
        vec2_scale(&movement->velocity, globals.time.delta, &timeVel);
        vec2_add(&transform->position, &timeVel, &transform->position);

        transform->rotation += movement->angularVelocity * globals.time.delta;

        if (movement->constrainToCamera) {
            camera_contrain(&globals.camera, &transform->position);
            transform->position.x += globals.scrollSpeed * globals.time.delta;
        }
    }
}