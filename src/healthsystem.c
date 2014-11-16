#include "healthsystem.h"

HealthSystem* health_system_new(EntityManager* entityManager) {
    HealthSystem* self = (HealthSystem*)calloc(1, sizeof(HealthSystem));

    aspect_system_init(&self->super, entityManager, COMPONENT_HEALTH);

    return self;
}

void health_system_update(HealthSystem* self) {
    EntityList entities;
    entity_list_init(&entities, 64);
    aspect_system_entities((AspectSystem*)self, &entities);

    for (u32 i = 0; i < entities.size; ++i) {
        // Entity* entity = (Entity*)vector_index(entities, i);

        // HealthComponent* health = (HealthComponent*)entities_get_component(
        //     self->super.entityManager,
        //     COMPONENT_HEALTH,
        //     entity);
    }
}

void health_system_render(HealthSystem* self) {
    // todo
}