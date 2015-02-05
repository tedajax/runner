#include "profiler.h"
#include "gametime.h"
#include "snprintf.h"
#include <time.h>

Profile* add_profile(const char* name) {
    if (count == capacity) {
        capacity <<= 1;
        profiles = (Profile*)realloc(profiles, sizeof(Profile) * capacity);
    }

    Profile* p = &profiles[count];

    p->name = (char*)name;

    p->capacity = PROFILER_STORED_SAMPLES;
    p->recent = (u64*)calloc(p->capacity, sizeof(u64));
    p->count = 0;

    p->peak = 0;
    p->average = 0;
    p->tick = 0;

    ++count;

    return p;
}

Profile* find_profile(const char* name) {
    for (u32 i = 0; i < count; ++i) {
        Profile* p = &profiles[i];

        if (strcmp(p->name, name) == 0) {
            return p;
        }
    }

    return add_profile(name);
}

void profile_add_sample(Profile* p, u64 time) {
    if (p->count >= p->capacity) {
        for (u32 i = 0; i < p->capacity - 1; ++i) {
            p->recent[i] = p->recent[i + 1];
        }
        p->recent[p->capacity - 1] = time;
    } else {
        p->recent[p->count] = time;
        ++p->count;
    }

    if (time > p->peak) {
        p->peak = time;
    }

    u64 avg = 0;
    for (u32 i = 0; i < p->count; ++i) {
        avg += p->recent[i];
    }
    p->average = avg / p->count;
}

void profile_dump(Profile* p, FILE* file, bool full) {
    fprintf(file, "%s:\n", p->name);
    fprintf(file, "\taverage: %luns\n\tpeak: %luns\n", p->average, p->peak);

    if (full) {
        fprintf(file, "\tsample count: %u\n", p->count);
        for (u32 i = 0; i < p->count; ++i) {
            fprintf(file, "\t[%u]: %lu\n", i, p->recent[i]);
        }
    }
    fprintf(file, "\n");
}

void profiler_init(bool enabled) {
#if PROFILING_ENABLED
    profilingEnabled = enabled;
    if (!enabled) {
        return;
    }
    capacity = 64;
    count = 0;
    profiles = (Profile*)calloc(capacity, sizeof(Profile));
#endif
}

void profiler_terminate() {
#if PROFILING_ENABLED
    for (u32 i = 0; i < count; ++i) {
        free(profiles[i].recent);
    }
    free(profiles);
#endif
}

void profiler_dump(FILE* file, bool full) {
#if PROFILING_ENABLED
    fprintf(file, "Runner Profile Dump:\n\n");

    for (u32 i = 0; i < count; ++i) {
        profile_dump(&profiles[i], file, full);
    }
#endif
}

void profiler_dump_log() {
#if PROFILING_ENABLED
    char buffer[128];
    snprintf(buffer, 128, "profiler_%ld.profile", time(NULL));
    FILE* fp = fopen(buffer, "w");
    profiler_dump(fp, true);
    fclose(fp);
#endif
}

void profiler_tick(const char* name) {
#if PROFILING_ENABLED
    if (!profilingEnabled) {
        return;
    }
    Profile* p = find_profile(name);

    p->tick = game_time_now();
#endif
}

void profiler_tock(const char* name) {
#if PROFILING_ENABLED
    if (!profilingEnabled) {
        return;
    }
    Profile* p = find_profile(name);

    u64 tock = game_time_now();
    u64 ns = tock - p->tick;
    profile_add_sample(p, ns);
#endif
}