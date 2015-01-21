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

#define MESSAGE_PARAM_BLOCK_SIZE 64

typedef struct message_on_collision_params_t {
    Entity other;
    u8 padding[MESSAGE_PARAM_BLOCK_SIZE - 4];
} MessageOnCollisionParams;

typedef struct message_on_damage_params_t {
    i32 damage;
    u8 padding[MESSAGE_PARAM_BLOCK_SIZE - 4];
} MessageOnDamageParams;

// TODO: Figure out some way of enforcing a schema on message params.  Right now it's really just built on trust.
typedef struct message_t {
    MessageType type;
    u8 paramBlock[MESSAGE_PARAM_BLOCK_SIZE];
} Message;

#define MESSAGE_SET_PARAM_BLOCK(msg, params)                    \
    memcpy(msg.paramBlock, ((u8*)((void*)&params)), MESSAGE_PARAM_BLOCK_SIZE)

typedef void(*message_cb)(Component*, const Message);
typedef void(*system_message_cb)(AspectSystem*, Entity, const Message);

static inline void message_init(Message* self, MessageType type) {
    self->type = type;
}

inline void message_copy(const Message* source, Message* dest) {
    dest->type = source->type;
    memcpy(dest->paramBlock, source->paramBlock, MESSAGE_PARAM_BLOCK_SIZE);
}

#endif