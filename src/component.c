#include "component.h"

void component_init(Component* self, ComponentType type) {
    self->type = type;
}