#include "healthcomponent.h"

HealthComponent* health_component_new(Entity entity, i32 maxHealth) {
    HealthComponent* self = CALLOC(1, HealthComponent);

    component_init((Component*)self, COMPONENT_HEALTH, sizeof(HealthComponent), entity);

    self->maxHealth = maxHealth;
    self->currentHealth = maxHealth;

    return self;
}

COMPONENT_DESERIALIZE(COMPONENT_HEALTH) {
    //TODO
    return NULL;
}

void health_component_on_collision_enter(Component* component, const Message msg) {
    HealthComponent* self = (HealthComponent*)component;

    self->currentHealth -= 40;
}