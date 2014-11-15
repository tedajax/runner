#ifndef RUNNER_GLOBALS_H
#define RUNNER_GLOBALS_H

#include "types.h"

// All globals should exist within the globals struct
// Try to limit total number of globals

typedef struct globals {
    u32 randomSeed;
} Globals;

// Sets all the values in globals to reasonable defaults (0 most likely)
// Not intended to use as a configuration step.
void globals_init();

extern Globals globals;

#endif