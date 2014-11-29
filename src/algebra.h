#ifndef RUNNER_ALGEBRA_H
#define RUNNER_ALGEBRA_H

#include "types.h"
#include "debug.h"
#include "log.h"
#include "inline.h"

#include <stdlib.h>
#include <math.h>

typedef struct SDL_Rect SDL_Rect;

extern const f32 DEG_TO_RAD;
extern const f32 RAD_TO_DEG;
extern const f32 PI;
extern const f32 PI_OVER_2;
extern const f32 TWO_PI;

f32 clampf(f32 value, f32 min, f32 max);
f32 lerpf(f32 start, f32 end, f32 t);
f32 sqrf(f32 n);
f32 randf(f32 max);
f32 randf_range(f32 min, f32 max);

typedef struct vec2_t {
    f32 x, y;
} Vec2;

typedef struct circle_t {
    Vec2 position;
    f32 radius;
} Circle;

typedef struct rect_t {
    Vec2 position;
    f32 width, height;
} Rect;

// Vec2 {
Vec2* vec2_new(const f32 x, const f32 y);
Vec2 vec2_init(const f32 x, const f32 y);
Vec2 vec2_clone(const Vec2* vec);
void vec2_set(Vec2* self, const f32 x, const f32 y);
void vec2_copy_to(const Vec2* source, Vec2* dest);
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

Vec2 vec2_zero();
Vec2 vec2_one();
Vec2 vec2_unit_x();
Vec2 vec2_unit_y();

Vec2 vec2_rand_range(f32 minx, f32 miny, f32 maxx, f32 maxy);
Vec2 vec2_rand_direction();

// } Vec2

// Rect {
Rect* rect_new(const Vec2* pos, const f32 w, const f32 h);
Rect rect_init(const Vec2* pos, const f32 w, const f32 h);
Rect rect_clone(const Rect* rect);
void rect_copy_sdl_rect(const SDL_Rect* source, Rect* dest);
void rect_set(Rect* self, const Vec2* pos, const f32 w, const f32 h);
void rect_copy_to(const Rect* source, Rect* dest);
void rect_to_sdl_rect(const Rect* source, SDL_Rect* dest);
bool rect_contains(const Rect* self, const Vec2* point);
bool rect_intersects(const Rect* self, const Rect* other);
bool rect_intersects_circle(const Rect* self, const Circle* other);

static inline f32 rect_left(const Rect* self) {
    return self->position.x;
}

static inline f32 rect_right(const Rect* self) {
    return self->position.x + self->width;
}

static inline f32 rect_top(const Rect* self) {
    return self->position.y;
}

static inline f32 rect_bottom(const Rect* self) {
    return self->position.y + self->height;
}
// } Rect

// Circle {
Circle* circle_new(const Vec2* pos, const f32 r);
Circle circle_init(const Vec2* pos, const f32 r);
Circle circle_clone(const Circle* circle);
void circle_set(Circle* self, const Vec2* pos, const f32 r);
void circle_copy_to(const Circle* source, Circle* dest);
void circle_bounds(const Circle* self, Rect* dest);
bool circle_contains(const Circle* self, const Vec2* point);
bool circle_intersects(const Circle* self, const Circle* other);
bool circle_intersects_rect(const Circle* self, const Rect* other);
// } Circle

#endif
