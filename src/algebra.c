#include "algebra.h"

const f32 DEG_TO_RAD =  0.017453f;
const f32 RAD_TO_DEG =  57.29578f;
const f32 PI =          3.141593f;
const f32 PI_OVER_2 =   1.570796f;
const f32 TWO_PI =      6.283185f;

f32 clampf(f32 value, f32 min, f32 max) {
    if (value < min) { value = min; }
    else if (value > max) { value = max; }

    return value;
}

f32 lerpf(f32 start, f32 end, f32 t) {
    return start + (end - start) * t;
}

f32 sqrf(f32 n) {
    return n * n;
}

f32 randf(f32 max) {
    f32 r = (f32)rand() / (f32)RAND_MAX;
    return r * max;
}

f32 randf_range(f32 min, f32 max) {
    f32 r = (f32)rand() / (f32)RAND_MAX;
    return lerpf(min, max, r);
}

Vec2* vec2_new(const f32 x, const f32 y) {
    Vec2* self = (Vec2 *)calloc(1, sizeof(Vec2));

    self->x = x;
    self->y = y;

    return self;
}

Vec2 vec2_init(const f32 x, const f32 y) {
    Vec2 self;
    vec2_set(&self, x, y);
    return self;
}

void vec2_set(Vec2* self, const f32 x, const f32 y) {
    ASSERT(self, "");

    self->x = x;
    self->y = y;
}

void vec2_copy_to(const Vec2* source, Vec2* dest) {
    dest->x = source->x;
    dest->y = source->y;
}

void vec2_add(const Vec2* lhs, const Vec2* rhs, Vec2* dest) {
    ASSERT(lhs && rhs && dest, "");

    dest->x = lhs->x + rhs->x;
    dest->y = lhs->y + rhs->y;
}

void vec2_sub(const Vec2* lhs, const Vec2* rhs, Vec2* dest) {
    ASSERT(lhs && rhs && dest, "");

    dest->x = lhs->x - rhs->x;
    dest->y = lhs->y - rhs->y;
}

void vec2_mul(const Vec2* lhs, const Vec2* rhs, Vec2* dest) {
    ASSERT(lhs && rhs && dest, "");

    dest->x = lhs->x * rhs->x;
    dest->y = lhs->y * rhs->y;
}

void vec2_scale(const Vec2* lhs, const f32 rhs, Vec2* dest) {
    ASSERT(lhs && dest, "");

    dest->x = lhs->x * rhs;
    dest->y = lhs->y * rhs;
}

void vec2_negate(const Vec2* vec, Vec2* dest) {
    ASSERT(vec && dest, "");

    dest->x = -vec->x;
    dest->y = -vec->y;
}

void vec2_normalize(const Vec2* vec, Vec2* dest) {
    ASSERT(vec && dest, "");

    f32 l = vec2_length(vec);

    dest->x = vec->x / l;
    dest->y = vec->y / l;
}

void vec2_lerp(const Vec2* start, const Vec2* end, const f32 t, Vec2* dest) {
    ASSERT(start && end && dest, "");

    dest->x = lerpf(start->x, end->x, t);
    dest->y = lerpf(start->y, end->y, t);
}

void vec2_direction(const f32 angle, Vec2* dest) {
    ASSERT(dest, "");

    dest->x = cosf(angle);
    dest->y = sinf(angle);
}

f32 vec2_length(const Vec2* vec) {
    return sqrtf(vec2_lengthSqr(vec));
}

f32 vec2_lengthSqr(const Vec2* vec) {
    return sqrf(vec->x) + sqrf(vec->y);
}

f32 vec2_dist(const Vec2* lhs, const Vec2* rhs) {
    return sqrtf(vec2_distSqr(lhs, rhs));
}

f32 vec2_distSqr(const Vec2* lhs, const Vec2* rhs) {
    return sqrf(rhs->x - lhs->x) + sqrf(rhs->y - lhs->y);
}

Vec2 vec2_zero() {
    Vec2 result;
    vec2_set(&result, 0.f, 0.f);
    return result;
}

Vec2 vec2_one() {
    Vec2 result;
    vec2_set(&result, 1.f, 1.f);
    return result;
}

Vec2 vec2_unit_x() {
    Vec2 result;
    vec2_set(&result, 1.f, 0.f);
    return result;
}

Vec2 vec2_unit_y() {
    Vec2 result;
    vec2_set(&result, 0.f, 1.f);
    return result;
}