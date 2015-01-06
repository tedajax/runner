#include "range.h"
#include "config.h"

void range_init(Range* self, f32 min, f32 max) {
    self->min = min;
    self->max = max;
}

void range_load(Range* self, const char* config, const char* section, const char* key) {
    //Ini* cfg = config_get(config);
    //self->min = ini_get_float_at(cfg, section, key, 0);
    //self->max = ini_get_float_at(cfg, section, key, 1);
}
