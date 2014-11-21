#include "component.h"
#include "components.h"

void component_init(Component* self, ComponentType type) {
    self->type = type;

    for (size_t i = 0; i < MESSAGE_LAST; ++i) {
        self->handlers[i] = NULL;
    }
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

void component_send_message(Component* self, Message* message) {
    if (self->handlers[message->type]) {
        self->handlers[message->type](self, message);
    }
}