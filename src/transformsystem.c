#include "transformsystem.h"

void transform_system_init(TransformSystem* self, EntityManager* entityManager) {
    aspect_system_init(&self->super, entityManager, COMPONENT_TRANSFORM, MAX_ENTITIES);
}