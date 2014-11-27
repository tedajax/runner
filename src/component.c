#include "component.h"
#include "components.h"

void component_init(Component* self, ComponentType type) {
    self->type = type;
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
