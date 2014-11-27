#include "aspectsystem.h"

AspectSystem* aspect_system_new(EntityManager* entityManager, ComponentType type) {
    AspectSystem* self = (AspectSystem*)calloc(1, sizeof(AspectSystem));

    self->entityManager = entityManager;
    self->systemType = type;

    REGISTER_SYSTEM(self->entityManager, self);

    for (u32 i = 0; i < MESSAGE_LAST; ++i) {
        self->handlers[i] = NULL;
    }

    return self;
}

void aspect_system_init(AspectSystem* self, EntityManager* entityManager, ComponentType type) {
    self->entityManager = entityManager;
    self->systemType = type;

    REGISTER_SYSTEM(self->entityManager, self);
}

void aspect_system_entities(AspectSystem* self, EntityList* dest) {
    entities_get_all_of(self->entityManager, self->systemType, dest);
}

void aspect_system_send_message(AspectSystem* self, Entity* entity, const Message message) {
    if (self->handlers[message.type]) {
        self->handlers[message.type](self, entity, message);
    }
}