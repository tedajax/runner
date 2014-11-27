#include "healthcomponent.h"

HealthComponent* health_component_new(i32 maxHealth) {
    HealthComponent* self = CALLOC(1, HealthComponent);

    component_init((Component*)self, COMPONENT_HEALTH);

    self->maxHealth = maxHealth;
    self->currentHealth = maxHealth;

    return self;
}

void health_component_on_collision_enter(Component* component, const Message msg) {
    HealthComponent* self = (HealthComponent*)component;

    self->currentHealth -= 40;
}