#ifndef RUNNER_COLLIDER_COMPONENT_H
#define RUNNER_COLLIDER_COMPONENT_H

#include "core.h"
#include "component.h"
#include "collider.h"

typedef struct collider_component_t {
    Component super;
    Collider collider;
} ColliderComponent;

ColliderComponent* collider_component_new(Entity entity, Collider* collider);
COMPONENT_DESERIALIZE(COMPONENT_COLLIDER);
int collider_component_compare(Component* c1, Component* c2);

#endif