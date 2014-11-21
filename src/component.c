#include "component.h"

void component_init(Component* self, ComponentType type) {
    self->type = type;

    for (size_t i = 0; i < MESSAGE_LAST; ++i) {
        self->handlers[i] = NULL;
    }
}

void component_send_message(Component* self, Message* message) {
    if (self->handlers[message->type]) {
        self->handlers[message->type](self, message);
    }
}