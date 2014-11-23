#include "healthsystem.h"

HealthSystem* health_system_new(EntityManager* entityManager) {
    HealthSystem* self = (HealthSystem*)calloc(1, sizeof(HealthSystem));

    aspect_system_init(&self->super, entityManager, COMPONENT_HEALTH);

    return self;
}

void health_system_update(HealthSystem* self, EntityList* entities) {
    aspect_system_entities((AspectSystem*)self, entities);

    for (u32 i = 0; i < entities->size; ++i) {
        Entity* entity = &entities->list[i];

        HealthComponent* health = (HealthComponent*)entities_get_component(
            self->super.entityManager,
            COMPONENT_HEALTH,
            entity);

        REQUIRED_COMPONENTS(health);

        if (health->currentHealth <= 0) {
            entities_remove_entity(self->super.entityManager, entity);
        }
    }
}

void health_system_render(HealthSystem* self, EntityList* entities) {
    // todo
}