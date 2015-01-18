#ifndef RUNNER_ASPECT_SYSTEM_H
#define RUNNER_ASPECT_SYSTEM_H

#include "core.h"
#include "entitymanager.h"
#include "messaging.h"

#define GET_ENTITY_LIST()                                       \
    EntityList entities;                                        \
    entity_list_init(&entities, 64);                            \
    aspect_system_entities((AspectSystem*)self, &entities);

#define GET_COMPONENT(entity, comptype)         \
        entities_get_component(                 \
        self->super.entityManager,              \
        comptype,                               \
        entity);                                \

#define REQUIRED_COMPONENTS(compexpr) MULTILINE_MACRO_BEGIN()                 \
    ASSERT(compexpr, "Entity does not have required components for system."); \
    MULTILINE_MACRO_END()

#define REGISTER_SYSTEM_HANDLER(type, func) \
    ((AspectSystem*)self)->handlers[type] = func

typedef struct aspect_system_t {
    EntityManager* entityManager;
    ComponentType systemType;
    system_message_cb handlers[MESSAGE_LAST];
} AspectSystem;

void aspect_system_init(AspectSystem* self, EntityManager* entityManager, ComponentType type);
void aspect_system_entities(AspectSystem* self, EntityList* dest);
void aspect_system_send_message(AspectSystem* self, Entity entity, const Message message);

#endif