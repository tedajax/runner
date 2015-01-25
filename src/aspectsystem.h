#ifndef RUNNER_ASPECT_SYSTEM_H
#define RUNNER_ASPECT_SYSTEM_H

#include "core.h"
#include "entitymanager.h"
#include "messaging.h"

#define GET_SYSTEM_COMPONENTS(self)                 \
    ComponentList* components = aspect_system_components((AspectSystem*)self)

#define GET_ENTITY(index)                   \
    components->components[index]->entity

#define GET_SYSTEM_COMPONENT(index)         \
    components->components[index]

#define GET_COMPONENT(entity, comptype)                \
        entities_get_component(                 \
            self->super.entityManager,          \
            comptype,                           \
            entity);

#define REQUIRED_COMPONENTS9(_1, _2, _3, _4, _5, _6, _7, _8, _9) MULTILINE_MACRO_BEGIN()                        \
    ASSERT(_1 && _2 && _3 && _4 && _5 && _6 && _7 && _8 && _9, "Entity does not have required components.");    \
    MULTILINE_MACRO_END()

#define REQUIRED_COMPONENTS8(_1, _2, _3, _4, _5, _6, _7, _8) MULTILINE_MACRO_BEGIN()                        \
    ASSERT(_1 && _2 && _3 && _4 && _5 && _6 && _7 && _8, "Entity does not have required components.");      \
    MULTILINE_MACRO_END()

#define REQUIRED_COMPONENTS7(_1, _2, _3, _4, _5, _6, _7) MULTILINE_MACRO_BEGIN()                        \
    ASSERT(_1 && _2 && _3 && _4 && _5 && _6 && _7, "Entity does not have required components.");        \
    MULTILINE_MACRO_END()

#define REQUIRED_COMPONENTS6(_1, _2, _3, _4, _5, _6) MULTILINE_MACRO_BEGIN()                        \
    ASSERT(_1 && _2 && _3 && _4 && _5 && _6, "Entity does not have required components.");          \
    MULTILINE_MACRO_END()

#define REQUIRED_COMPONENTS5(_1, _2, _3, _4, _5) MULTILINE_MACRO_BEGIN()                        \
    ASSERT(_1 && _2 && _3 && _4 && _5, "Entity does not have required components.");            \
    MULTILINE_MACRO_END()

#define REQUIRED_COMPONENTS4(_1, _2, _3, _4) MULTILINE_MACRO_BEGIN()                        \
    ASSERT(_1 && _2 && _3 && _4, "Entity does not have required components.");              \
    MULTILINE_MACRO_END()

#define REQUIRED_COMPONENTS3(_1, _2, _3) MULTILINE_MACRO_BEGIN()                        \
    ASSERT(_1 && _2 && _3, "Entity does not have required components.");                \
    MULTILINE_MACRO_END()

#define REQUIRED_COMPONENTS2(_1, _2) MULTILINE_MACRO_BEGIN()                        \
    ASSERT(_1 && _2, "Entity does not have required components.");                  \
    MULTILINE_MACRO_END()

#define REQUIRED_COMPONENTS1(_1) MULTILINE_MACRO_BEGIN()                        \
    ASSERT(_1, "Entity does not have required components.");                    \
    MULTILINE_MACRO_END()

#define GET_REQUIRED_COMPONENTS_MACRO(_1, _2, _3, _4, _5, _6, _7, _8, _9, NAME, ...) NAME

#ifdef _MSC_VER
    // Microsoft is retarded and decided that their implementation of __VA_ARGS__ is correct
    // despite all other compilers doing exactly the opposite of what they do so we have to do this stupid work around.
    #define EXPAND(x) x
    #define REQUIRED_COMPONENTS(...) EXPAND(GET_REQUIRED_COMPONENTS_MACRO(__VA_ARGS__, REQUIRED_COMPONENTS9, REQUIRED_COMPONENTS8, REQUIRED_COMPONENTS7, REQUIRED_COMPONENTS6, REQUIRED_COMPONENTS5, REQUIRED_COMPONENTS4, REQUIRED_COMPONENTS3, REQUIRED_COMPONENTS2, REQUIRED_COMPONENTS1))EXPAND((__VA_ARGS__))
#else
    #define REQUIRED_COMPONENTS(...) GET_REQUIRED_COMPONENTS_MACRO(__VA_ARGS__, REQUIRED_COMPONENTS9, REQUIRED_COMPONENTS8, REQUIRED_COMPONENTS7, REQUIRED_COMPONENTS6, REQUIRED_COMPONENTS5, REQUIRED_COMPONENTS4, REQUIRED_COMPONENTS3, REQUIRED_COMPONENTS2, REQUIRED_COMPONENTS1)(__VA_ARGS__)
#endif

#define REGISTER_SYSTEM_HANDLER(type, func) \
    ((AspectSystem*)self)->handlers[type] = func

typedef struct aspect_system_t {
    EntityManager* entityManager;
    ComponentType systemType;
    system_message_cb handlers[MESSAGE_LAST];
} AspectSystem;

void aspect_system_init(AspectSystem* self, EntityManager* entityManager, ComponentType type);
ComponentList* aspect_system_components(AspectSystem* self);
void aspect_system_send_message(AspectSystem* self, Entity entity, const Message message);

#endif