#ifndef RUNNER_ALGEBRA_H
#define RUNNER_ALGEBRA_H

#include "types.h"
#include "debug.h"
#include "log.h"

#include <stdlib.h>
#include <math.h>

extern const f32 DEG_TO_RAD;
extern const f32 RAD_TO_DEG;
extern const f32 PI;
extern const f32 PI_OVER_2;
extern const f32 TWO_PI;

f32 clampf(f32 value, f32 min, f32 max);
f32 lerpf(f32 start, f32 end, f32 t);
f32 sqrf(f32 n);

// Vec2 {
typedef struct vec2_t {
    f32 x, y;
} Vec2;

Vec2* vec2_new(const f32 x, const f32 y);
void vec2_set(Vec2* self, const f32 x, const f32 y);
void vec2_add(const Vec2* lhs, const Vec2* rhs, Vec2* dest);
void vec2_sub(const Vec2* lhs, const Vec2* rhs, Vec2* dest);
void vec2_mul(const Vec2* lhs, const Vec2* rhs, Vec2* dest);
void vec2_scale(const Vec2* lhs, const f32 rhs, Vec2* dest);
void vec2_negate(const Vec2* vec, Vec2* dest);
void vec2_normalize(const Vec2* vec, Vec2* dest);
void vec2_lerp(const Vec2* start, const Vec2* end, const f32 t, Vec2* dest);
void vec2_direction(const f32 angle, Vec2* dest);
f32 vec2_length(const Vec2* vec);
f32 vec2_lengthSqr(const Vec2* vec);
f32 vec2_dist(const Vec2* lhs, const Vec2* rhs);
f32 vec2_distSqr(const Vec2* lhs, const Vec2* rhs);
// } Vec2

#endif