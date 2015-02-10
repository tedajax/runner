#include "gravitysystem.h"

void gravity_system_init(GravitySystem* self, EntityManager* entityManager) {
    aspect_system_init(&self->super, entityManager, COMPONENT_GRAVITY, 0);
}

void gravity_system_update(GravitySystem* self) {
    GET_SYSTEM_COMPONENTS(self);

    for (u32 i = 0; i < components->count; ++i) {
        Entity entity = GET_ENTITY(i);

        GravityComponent* gravity = (GravityComponent*)GET_SYSTEM_COMPONENT(i);

        MovementComponent* movement = (MovementComponent*)GET_COMPONENT(entity, COMPONENT_MOVEMENT);

        REQUIRED_COMPONENTS(gravity, movement);

        Vec2 gravScale;
        vec2_copy_to(&gravity->gravAccel, &gravScale);
        vec2_scale(&gravScale, globals.time.delta, &gravScale);

        vec2_add(&movement->velocity, &gravScale, &movement->velocity);
        //printf("%f\n", movement->velocity.y);
    }
}