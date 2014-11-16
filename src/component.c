#include "component.h"

Component* component_new(ComponentType type) {
    Component* self = (Component*)calloc(1, sizeof(Component));
    self->type = type;
    return self;
}