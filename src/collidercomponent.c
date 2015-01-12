#include "collidercomponent.h"

ColliderComponent* collider_component_new(Collider* collider) {
    ColliderComponent* self = CALLOC(1, ColliderComponent);

    component_init((Component*)self, COMPONENT_COLLIDER);

    self->collider = *collider;

    return self;
}
