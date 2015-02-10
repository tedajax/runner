#include "healthsystem.h"

void health_system_init(HealthSystem* self, EntityManager* entityManager) {
    aspect_system_init(&self->super, entityManager, COMPONENT_HEALTH, 64);

    REGISTER_SYSTEM_HANDLER(MESSAGE_DAMAGE, health_system_on_damage);
}

void health_system_update(HealthSystem* self) {
    GET_SYSTEM_COMPONENTS(self);

    for (u32 i = 0; i < components->count; ++i) {
        Entity entity = GET_ENTITY(i);

        HealthComponent* health = (HealthComponent*)GET_SYSTEM_COMPONENT(i);

        REQUIRED_COMPONENTS(health);

        if (health->currentHealth <= 0) {
            entities_remove_entity(self->super.entityManager, entity);
        }
    }
}

void health_system_on_damage(AspectSystem* system, Entity entity, Message message) {
    HealthSystem* self = (HealthSystem*)system;

    HealthComponent* health =
        (HealthComponent*)GET_COMPONENT(entity, COMPONENT_HEALTH);

    REQUIRED_COMPONENTS(health);

    i32 damage = ((MessageOnDamageParams*)message.paramBlock)->damage;

    health->currentHealth -= damage;
}