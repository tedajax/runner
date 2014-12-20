#ifndef RUNNER_PROFILER_H
#define RUNNER_PROFILER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"

typedef struct profile_t {
    char* name;
    u64* recent;
    u32 capacity;
    u32 count;
    u64 average;
    u64 peak;
    u64 tick;
} Profile;

Profile* profiles;
u32 capacity;
u32 count;

void profiler_init();
void profiler_terminate();
void profiler_dump(FILE* file);
void profiler_tick(const char* name);
void profiler_tock(const char* name);

#endif