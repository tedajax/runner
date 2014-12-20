#include "bulletsource.h"
#include "entitymanager.h"
#include "entityfactory.h"
#include "texturemanager.h"

void bullet_source_init(BulletSource* self, Vec2* offset, char* config, char* section) {
    self->active = false;
    self->fireTimer = 0.f;
    self->level = 0;
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
    self->textureName = ini_get_string_at(cfg, section, "texture_name", level);
}

void bullet_source_update(BulletSource* self, f32 dt, EntityManager* entityManager, Vec2* anchor) {
    if (self->fireTimer > 0.f) {
        self->fireTimer -= dt;
    }

    if (!self->active) {
        return;
    }

    if (self->fireTimer <= 0.f) {
        self->fireTimer = self->config.fireDelay;
        bullet_source_fire(self, entityManager, anchor);
    }
}

void bullet_source_fire(BulletSource* self, EntityManager* entityManager, Vec2* anchor) {
    // TODO multiple bullets

    for (u32 i = 0; i < self->config.count; ++i) {
        Vec2 pos;
        vec2_add(anchor, &self->offset, &pos);
        BulletConfig config;        
        config.startSpeed = self->config.speed;
        f32 sa = (i - (self->config.count / 2)) * self->config.spread + self->config.startAngle;
        config.startAngle = sa;
        config.acceleration = self->config.acceleration;
        config.rotationRate = self->config.rotationRate;
        config.lifetime = self->config.lifetime;
        //TODO creating these in place is causing problems...
        entity_create_bullet(entityManager, &config, pos, textures_get(self->config.textureName));
    }
}