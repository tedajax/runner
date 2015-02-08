#ifndef RUNNER_PROFILER_H
#define RUNNER_PROFILER_H

#include <stdio.h>
#include "memory.h"
#include <string.h>

#include "types.h"

#define PROFILER_STORED_SAMPLES 4096

#define PROFILING_ENABLED 1

typedef struct profile_tick_t {
    char context[64];
    u64 time;
} ProfileTick;

typedef struct profile_t {
    char* name;
    ProfileTick* recent;
    u32 capacity;
    u32 count;
    u32 index;
    u64 average;
    u64 peak;
    u64 tick;
} Profile;

Profile* profiles;
u32 capacity;
u32 count;
bool profilingEnabled;

void profiler_init(bool enabled);
void profiler_terminate();
void profiler_dump(FILE* file, bool full);
void profiler_dump_log();
void profiler_tick(const char* name);
void profiler_tock(const char* name);
void profiler_tock_context(const char* name, char* context);

#endif