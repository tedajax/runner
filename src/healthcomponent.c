#include "healthcomponent.h"

HealthComponent* health_component_new(i32 maxHealth) {
    HealthComponent* self = (HealthComponent*)calloc(1, sizeof(HealthComponent));

    self->super.type = COMPONENT_HEALTH;
    self->maxHealth = maxHealth;
    self->currentHealth = maxHealth;

    return self;
}