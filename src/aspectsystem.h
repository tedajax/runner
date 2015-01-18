#ifndef RUNNER_ASPECT_SYSTEM_H
#define RUNNER_ASPECT_SYSTEM_H

#include "core.h"
#include "entitymanager.h"
#include "messaging.h"

#define GET_ENTITY_LIST()                                       \
    EntityList entities;                                        \
    entity_list_init(&entities, 64);                            \
    aspect_system_entities((AspectSystem*)self, &entities);

#define GET_COMPONENT2(entity, comptype)        \
        entities_get_component(                 \
            self->super.entityManager,          \
            comptype,                           \
            entity);

#define GET_COMPONENT3(entity, comptype, manager)   \
        entities_get_component(                     \
            manager,                                \
            comptype,                               \
            entity);

#define GET_GET_COMPONENT_MACRO(_1, _2, _3, NAME, ...) NAME

#ifdef _MSC_VER
    // Microsoft is retarded and decided that their implementation of __VA_ARGS__ is correct
    // despite all other compilers doing exactly the opposite of what they do so we have to do this stupid work around.
    #define EXPAND(x) x
    #define GET_COMPONENT(...) EXPAND(GET_GET_COMPONENT_MACRO(__VA_ARGS__, GET_COMPONENT3, GET_COMPONENT2, GET_COMPONENT1))EXPAND((__VA_ARGS__))
#else
    #define GET_COMPONENT(...) GET_GET_COMPONENT_MACRO(__VA_ARGS__, GET_COMPONENT3, GET_COMPONENT2, GET_COMPONENT1)(__VA_ARGS__)
#endif

#define REQUIRED_COMPONENTS(compexpr) MULTILINE_MACRO_BEGIN()                 \
    ASSERT(compexpr, "Entity does not have required components."); \
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