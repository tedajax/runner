#ifndef RUNNER_RANGE_H
#define RUNNER_RANGE_H

#include "types.h"
#include "algebra.h"

typedef struct range_t {
    f32 min;
    f32 max;
} Range;

void range_init(Range* self, f32 min, f32 max);
void range_load(Range* self, const char* config, const char* section, const char* key);

inline f32 range_rand(Range* self) {
    return randf_range(self->min, self->max);
}

inline f32 range_mid(Range* self) {
    return (self->max + self->min) / 2.f;
}

#endif