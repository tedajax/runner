#include "movementsystem.h"

MovementSystem* movement_system_new(EntityManager* entityManager) {
    MovementSystem* self = (MovementSystem*)calloc(1, sizeof(MovementSystem));

    aspect_system_init(&self->super, entityManager, COMPONENT_MOVEMENT);

    return self;
}

void movement_system_update(MovementSystem* self) {
    EntityList entities;
    entity_list_init(&entities, 64);
    aspect_system_entities((AspectSystem*)self, &entities);

    for (u32 i = 0; i < entities.size; ++i) {
        Entity entity = entities.list[i];

        TransformComponent* transform = GET_COMPONENT(TransformComponent, COMPONENT_TRANSFORM, entity);

        MovementComponent* movement = GET_COMPONENT(MovementComponent, COMPONENT_MOVEMENT, entity);

        if (transform && movement) {
            Vec2 timeVel;
            vec2_scale(&movement->velocity, globals.time.delta, &timeVel);
            vec2_add(&transform->position, &movement->velocity, &transform->position);

            transform->rotation += movement->angularVelocity * globals.time.delta;
        }
    }
}