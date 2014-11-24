#include "gravitysystem.h"

void gravity_system_init(GravitySystem* self, EntityManager* entityManager) {
    aspect_system_init(&self->super, entityManager, COMPONENT_GRAVITY);
}

void gravity_system_update(GravitySystem* self, EntityList* entities) {
    aspect_system_entities((AspectSystem*)self, entities);

    for (u32 i = 0; i < entities->size; ++i) {
        Entity entity = entities->list[i];

        MovementComponent* movement = (MovementComponent*)entities_get_component(
            self->super.entityManager,
            COMPONENT_MOVEMENT,
            &entity);

        GravityComponent* gravity = (GravityComponent*)entities_get_component(
            self->super.entityManager,
            COMPONENT_GRAVITY,
            &entity);

        if (!movement || !gravity) {
            continue;
        }

        Vec2 gravScale;
        vec2_copy_to(&gravity->gravAccel, &gravScale);
        vec2_scale(&gravScale, globals.time.delta, &gravScale);

        vec2_add(&movement->velocity, &gravScale, &movement->velocity);
        //printf("%f\n", movement->velocity.y);
    }
}