#include "range.h"
#include "config.h"

void range_init(Range* self, f32 min, f32 max) {
    self->min = min;
    self->max = max;
}
