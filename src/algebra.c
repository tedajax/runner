#include "algebra.h"

#include <SDL2/SDL_rect.h>

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

Vec2 vec2_clone(const Vec2* vec) {
    Vec2 result;
    vec2_set(&result, vec->x, vec->y);
    return result;
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

f32 vec2_dot(const Vec2* lhs, const Vec2* rhs) {
    return lhs->x * rhs->x + lhs->y * rhs->y;
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

Vec2 vec2_rand_range(f32 minx, f32 miny, f32 maxx, f32 maxy) {
    Vec2 result;
    vec2_set(&result, randf_range(minx, maxx), randf_range(miny, maxy));
    return result;
}

Vec2 vec2_rand_direction() {
    f32 angle = randf_range(0, TWO_PI);
    Vec2 result;
    vec2_direction(angle, &result);
    return result;
}

//////////////////////////////////////////////////////////////////////////
///////////////////         Rect          ////////////////////////////////
//////////////////////////////////////////////////////////////////////////

Rect* rect_new(const Vec2* pos, const f32 w, const f32 h) {
    Rect* self = (Rect*)calloc(1, sizeof(Rect));

    vec2_copy_to(pos, &self->position);

    self->width = w;
    self->height = h;

    return self;
}
Rect rect_init(const Vec2* pos, const f32 w, const f32 h) {
    Rect result;

    vec2_copy_to(pos, &result.position);

    result.width = w;
    result.height = h;

    return result;
}

Rect rect_clone(const Rect* rect) {
    return rect_init(&rect->position, rect->width, rect->height);
}

void rect_copy_sdl_rect(const SDL_Rect* source, Rect* dest) {
    vec2_set(&dest->position, (f32)source->x, (f32)source->y);
    dest->width = (f32)source->w;
    dest->height = (f32)source->h;
}

void rect_set(Rect* self, const Vec2* pos, const f32 w, const f32 h) {
    vec2_copy_to(pos, &self->position);
    self->width = w;
    self->height = h;
}

void rect_copy_to(const Rect* source, Rect* dest) {
    vec2_copy_to(&source->position, &dest->position);
    dest->width = source->width;
    dest->height = source->height;
}

void rect_to_sdl_rect(const Rect* source, SDL_Rect* dest) {
    dest->x = (i16)source->position.x;
    dest->y = (i16)source->position.y;
    dest->w = (i16)source->width;
    dest->h = (i16)source->height;
}

bool rect_contains(const Rect* self, const Vec2* point) {
    f32 l = rect_left(self);
    f32 r = rect_right(self);
    f32 t = rect_top(self);
    f32 b = rect_bottom(self);

    return point->x >= l && point->x <= r && point->y >= t && point->y <= b;
}

bool rect_intersects(const Rect* self, const Rect* other) {
    f32 l1 = rect_left(self);
    f32 r1 = rect_right(self);
    f32 t1 = rect_top(self);
    f32 b1 = rect_bottom(self);

    f32 l2 = rect_left(other);
    f32 r2 = rect_right(other);
    f32 t2 = rect_top(other);
    f32 b2 = rect_bottom(other);

    return l2 <= r1 && r2 >= l1 && t2 <= b1 && b2 >= t1;
}

bool rect_intersects_circle(const Rect* self, const Circle* other) {
    f32 l = rect_left(self) - other->radius;
    f32 r = rect_right(self) + other->radius;
    f32 t = rect_top(self) - other->radius;
    f32 b = rect_bottom(self) + other->radius;

    f32 px = other->position.x;
    f32 py = other->position.y;

    return px >= l && px <= r && py >= t && py <= b;
}

//////////////////////////////////////////////////////////////////////////
///////////////////         Circle        ////////////////////////////////
//////////////////////////////////////////////////////////////////////////

Circle* circle_new(const Vec2* pos, const f32 r) {
    Circle* self = (Circle*)calloc(1, sizeof(Circle));

    vec2_copy_to(pos, &self->position);
    self->radius = r;

    return self;
}

Circle circle_init(const Vec2* pos, const f32 r) {
    Circle result;
    vec2_copy_to(pos, &result.position);
    result.radius = r;
    return result;
}

Circle circle_clone(const Circle* circle) {
    return circle_init(&circle->position, circle->radius);
}

void circle_set(Circle* self, const Vec2* pos, const f32 r) {
    vec2_copy_to(pos, &self->position);
    self->radius = r;
}

void circle_copy_to(const Circle* source, Circle* dest) {
    vec2_copy_to(&source->position, &dest->position);
    dest->radius = source->radius;
}

void circle_bounds(const Circle* self, Rect* dest) {
    f32 r = self->radius;
    f32 d = r * 2.f;
    vec2_set(&dest->position, self->position.x - r, self->position.y - r);
    dest->width = d;
    dest->height = d;
}

bool circle_contains(const Circle* self, const Vec2* point) {
    f32 d = vec2_dist(&self->position, point);
    return d <= self->radius;
}

bool circle_intersects(const Circle* self, const Circle* other) {
    f32 d = vec2_dist(&self->position, &other->position);
    return d <= (self->radius + other->radius);
}

bool circle_intersects_rect(const Circle* self, const Rect* other) {
    return rect_intersects_circle(other, self);
}