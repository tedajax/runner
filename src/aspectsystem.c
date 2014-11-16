#include "aspectsystem.h"

AspectSystem* aspect_system_new(EntityManager* entityManager, ComponentType type) {
    AspectSystem* self = (AspectSystem*)calloc(1, sizeof(AspectSystem));

    self->entityManager = entityManager;
    self->systemType = type;

    return self;
}

void aspect_system_init(AspectSystem* self, EntityManager* entityManager, ComponentType type) {
    self->entityManager = entityManager;
    self->systemType = type;
}

void aspect_system_entities(AspectSystem* self, EntityList* dest) {
    entities_get_all_of(self->entityManager, self->systemType, dest);
}