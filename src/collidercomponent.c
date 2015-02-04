#include "collidercomponent.h"

ColliderComponent* collider_component_new(Entity entity, Collider* collider) {
    ColliderComponent* self = CALLOC(1, ColliderComponent);

    component_init((Component*)self, COMPONENT_COLLIDER, sizeof(ColliderComponent), entity);

    self->collider = *collider;

    return self;
}

ColliderComponent* collider_component_deserialize(Config* config, const char* table) {
    ColliderConfig colliderConfig;
    collider_config_deserialize((TypeConfig*)&colliderConfig, config, (char*)table);
    Collider collider;
    collider_init_config(&collider, 0, &colliderConfig);
    return collider_component_new(0, &collider);
}

int collider_component_compare(Component* c1, Component* c2) {
    ColliderComponent* cc1 = (ColliderComponent*)c1;
    ColliderComponent* cc2 = (ColliderComponent*)c2;
    f32 diff = rect_left(&cc1->collider.volume->bounds) - rect_left(&cc2->collider.volume->bounds);
    return (diff > 0.f) - (diff < 0.f);
}