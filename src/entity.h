#ifndef RUNNER_ENITTY_H
#define RUNNER_ENTITY_H

#include "core.h"

typedef struct entity_t {
    i32 id;
} Entity;

Entity* entity_new(i32 id);

#endif