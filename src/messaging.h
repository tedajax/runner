#ifndef RUNNER_MESSAGING_H
#define RUNNER_MESSAGING_H

#include "core.h"

typedef enum message_type_e {
    MESSAGE_TEST,
    MESSAGE_DAMAGE,
    MESSAGE_ON_COLLISION_ENTER,
    MESSAGE_ON_COLLISION_STAY,
    MESSAGE_ON_COLLISION_EXIT,
    MESSAGE_LAST
} MessageType;

#define MESSAGE_MAX_PARAMS 3

typedef struct message_t {
    MessageType type;
    void* params[MESSAGE_MAX_PARAMS];
} Message;

typedef struct component_t Component;
typedef void(*message_cb)(Component*, const Message*);

inline void message_copy(const Message* source, Message* dest) {
    dest->type = source->type;
    for (u32 i = 0; i < MESSAGE_LAST; ++i) {
        dest->params[i] = source->params[i];
    }
}

#endif