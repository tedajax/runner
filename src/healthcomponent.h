#ifndef RUNNER_HEALTH_COMPONENT_H
#define RUNNER_HEALTH_COMPONENT_H

#include "core.h"
#include "component.h"

typedef struct health_component_t {
    Component super;
    i32 maxHealth;
    i32 currentHealth;
} HealthComponent;

HealthComponent* health_component_new(Entity entity, i32 maxHealth);
COMPONENT_DEFINE(COMPONENT_HEALTH);

void health_component_on_collision_enter(Component* component, const Message msg);

#endif