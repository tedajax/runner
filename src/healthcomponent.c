#include "healthcomponent.h"

HealthComponent* health_component_new(i32 maxHealth) {
    HealthComponent* self = CALLOC(1, HealthComponent);

    component_init((Component*)self, COMPONENT_HEALTH);

    REGISTER_HANDLER(MESSAGE_DAMAGE, health_component_on_damage);
    REGISTER_HANDLER(MESSAGE_ON_COLLISION_ENTER, health_component_on_collision_enter);

    self->maxHealth = maxHealth;
    self->currentHealth = maxHealth;

    return self;
}

void health_component_on_damage(Component* component, const Message msg) {
    HealthComponent* self = (HealthComponent*)component;
    --self->currentHealth;
    printf("HEALTH DAMAGE! %d\n", self->currentHealth);
}

void health_component_on_collision_enter(Component* component, const Message msg) {
    HealthComponent* self = (HealthComponent*)component;

    self->currentHealth -= 40;
}