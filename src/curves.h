#ifndef RUNNER_CURVES_H
#define RUNNER_CURVES_H

#include "core.h"

typedef enum curve_type_e {
    CURVE_LINEAR,
    CURVE_CATMULLROM,
} CurveType;

typedef struct curve_t {
    Vec2* points;
    u32 capacity;
    u32 count;
    CurveType type;
} Curve;

void curve_init(Curve* self, u32 capacity, CurveType type);
void curve_add_point(Curve* self, Vec2 point);
void curve_evaluate()

#endif