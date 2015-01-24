#include "component.h"
#include "components.h"

void component_init(Component* self, ComponentType type, Entity entity) {
    self->type = type;
    self->entity = entity;
}

void component_free(Component* self) {
    switch (self->type) {
        default:
            free(self);
            break;

        case COMPONENT_BG_MANAGER:
            free(((BgManagerComponent*)self)->transforms);
            free(self);
            break;
    }
}

void component_free_void(void* self) {
    component_free((Component*)self);
}

int component_entity_compare(Component* c1, Component* c2) {
    return c1->entity - c2->entity;
}