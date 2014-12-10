#include "curves.h"

void curve_init(Curve* self, u32 capacity, CurveType type) {
    self->capacity = capacity;
    self->count = 0;
    self->points = CALLOC(self->capacity, Vec2);
    self->type = type;
}

void curve_add_control_point(Curve* self, Vec2 point) {
    ASSERT(self->count < self->capacity, "Max control points added.");

    vec2_copy_to(&point, &self->points[self->count]);
    ++self->count;
}

void curve_evaluate(Curve* self, f32 t, Vec2* dest) {
    ASSERT(t >= 0.f && t <= (f32)self->count, "Evalutation parameter out of range.");

    switch (self->type) {
        default:
        case CURVE_LINEAR: {
            f32 t_floor = floorf(t);
            int c0 = (int)t;
            int c1 = c0 + 1;
            vec2_lerp(&self->points[c0], &self->points[c1], t_floor, dest);
        }

        case CURVE_CATMULLROM:
            dest->x = 0.f;
            dest->y = 0.f;
    }
}