#include "healthsystem.h"

void health_system_init(HealthSystem* self, EntityManager* entityManager) {
    aspect_system_init(&self->super, entityManager, COMPONENT_HEALTH);

    REGISTER_SYSTEM_HANDLER(MESSAGE_DAMAGE, health_system_on_damage);
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

void health_system_on_damage(AspectSystem* system, Entity* entity, Message message) {
    HealthSystem* self = (HealthSystem*)system;

    HealthComponent* health =
        (HealthComponent*)entities_get_component(self->super.entityManager,
        COMPONENT_HEALTH,
        entity);

    REQUIRED_COMPONENTS(health);

    i32 damage = *(i32*)message.params[0];

    health->currentHealth -= damage;
}