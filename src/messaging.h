#ifndef RUNNER_MESSAGING_H
#define RUNNER_MESSAGING_H

#include "core.h"

//Forward declarations
typedef struct component_t Component;
typedef struct aspect_system_t AspectSystem;

// Each comment refers to the types of the params in the message.
typedef enum message_type_e {
    // Entity source, f32 amount, NULL
    MESSAGE_DAMAGE,

    // Entity other, NULL, NULL
    MESSAGE_ON_COLLISION_ENTER,

    // Entity other, NULL, NULL
    MESSAGE_ON_COLLISION_STAY,

    // Entity other, NULL, NULL
    MESSAGE_ON_COLLISION_EXIT,

    // Entity created, NULL, NULL
    MESSAGE_ENTITY_CREATED,

    // Entity removed, NULL, NULL
    MESSAGE_ENTITY_REMOVED,

    // N/A
    MESSAGE_LAST
} MessageType;

#define MESSAGE_MAX_PARAMS 3
#define MESSAGE_PARAM_BLOCK_SIZE 64

typedef struct message_on_collision_params_t {
    Entity other;
    u8 padding[MESSAGE_PARAM_BLOCK_SIZE - 4];
} MessageOnCollisionParams;

// TODO: Figure out some way of enforcing a schema on message params.  Right now it's really just built on trust.
typedef struct message_t {
    MessageType type;
    u32 paramCount;
    void* params[MESSAGE_MAX_PARAMS];
    u8 paramBlock[MESSAGE_PARAM_BLOCK_SIZE];
} Message;

typedef void(*message_cb)(Component*, const Message);
typedef void(*system_message_cb)(AspectSystem*, Entity, const Message);

static inline void message_init(Message* self, MessageType type) {
    self->type = type;
    self->paramCount = 0;
    for (u32 i = 0; i < MESSAGE_MAX_PARAMS; ++i) {
        self->params[i] = NULL;
    }
}

static inline void message_add_param(Message* self, void* param) {
    ASSERT(self->paramCount < MESSAGE_MAX_PARAMS, "Maximum params added to message.");

    self->params[self->paramCount] = param;
    ++self->paramCount;
}

inline void message_copy(const Message* source, Message* dest) {
    dest->type = source->type;
    for (u32 i = 0; i < MESSAGE_MAX_PARAMS; ++i) {
        dest->params[i] = source->params[i];
    }
}

#endif