#include "bulletsource.h"

void bullet_source_init(BulletSource* self, Vec2* offset, char* config, char* section) {
    self->active = false;
    self->fireTimer = 0.f;
    vec2_copy_to(offset, &self->offset);
    bullet_source_config(&self->config, config, section, 0);
}

void bullet_source_config(BulletSourceConfig* self, char* config, char* section, u32 level) {
    Ini* cfg = config_get(config);

    self->count = ini_get_int_at(cfg, section, "count", level);
    self->spread = ini_get_float_at(cfg, section, "spread", level);
    self->lifetime = ini_get_float_at(cfg, section, "lifetime", level);
    self->speed = ini_get_float_at(cfg, section, "speed", level);
    self->acceleration = ini_get_float_at(cfg, section, "acceleration", level);
    self->fireDelay = ini_get_float_at(cfg, section, "fire_delay", level);
    self->startAngle = ini_get_float_at(cfg, section, "start_angle", level);
    self->rotationRate = ini_get_float_at(cfg, section, "rotation_rate", level);
}

bool bullet_source_update(BulletSource* self, f32 dt) {
    if (self->fireTimer < self->config.fireDelay) {
        self->fireTimer += dt;
    }

    if (!self->active) {
        return false;
    }

    if (self->fireTimer >= self->config.fireDelay) {
        self->fireTimer = 0.f;
        return true;
    }

    return false;
}
