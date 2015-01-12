#include "bulletsource.h"
#include "entitymanager.h"
#include "entityfactory.h"
#include "texturemanager.h"

void bullet_source_init(BulletSource* self, char* config, char* section) {
    self->active = false;
    self->fireTimer = 0.f;
    self->level = 0;
    bullet_source_config(&self->config, config, section, 0);
}

void bullet_source_config(BulletSourceConfig* self, char* config, char* section, u32 level) {
 //   Ini* cfg = config_get(config);

 //   self->offset = ini_get_vec2_at(cfg, section, "offset", level);
 //   self->count = ini_get_int_at(cfg, section, "count", level);
 //   self->spread = dynf32_config_at(cfg, section, "spread", level);
 //   self->lifetime = dynf32_config_at(cfg, section, "lifetime", level);
 //   self->speed = dynf32_config_at(cfg, section, "speed", level);
 //   self->angle = dynf32_config_at(cfg, section, "angle", level);
 //   self->fireDelay = dynf32_config_at(cfg, section, "fire_delay", level);
 //   self->startAngle = dynf32_config_at(cfg, section, "start_angle", level);
 //   self->damage = dynf32_config_at(cfg, section, "damage", level);
 //   self->textureName = ini_get_string_at(cfg, section, "texture_name", level);

 //   dynf32_start_tween(&self->spread, &globals.tweens);
 //   dynf32_start_tween(&self->lifetime, &globals.tweens);
 //   dynf32_start_tween(&self->speed, &globals.tweens);
 //   dynf32_start_tween(&self->angle, &globals.tweens);
 //   dynf32_start_tween(&self->fireDelay, &globals.tweens);
 //   dynf32_start_tween(&self->startAngle, &globals.tweens);
 //   dynf32_start_tween(&self->damage, &globals.tweens);

    //char* collider = ini_get_string_at(cfg, section, "collider", level);
    //collider_config_init(&self->colliderConfig, config, collider);
}

void bullet_source_release(BulletSource* self) {
    dynf32_release(&self->config.spread);
    dynf32_release(&self->config.lifetime);
    dynf32_release(&self->config.speed);
    dynf32_release(&self->config.angle);
    dynf32_release(&self->config.fireDelay);
    dynf32_release(&self->config.startAngle);
    dynf32_release(&self->config.damage);
}

void bullet_source_update(BulletSource* self, f32 dt, EntityManager* entityManager, Vec2* anchor) {
    if (self->fireTimer > 0.f) {
        self->fireTimer -= dt;
    }

    if (!self->active) {
        return;
    }

    if (self->fireTimer <= 0.f) {
        self->fireTimer = dynf32_get(&self->config.fireDelay);
        bullet_source_fire(self, entityManager, anchor);
    }
}

void bullet_source_fire(BulletSource* self, EntityManager* entityManager, Vec2* anchor) {
    for (i32 i = 0; i < self->config.count; ++i) {
        Vec2 pos;
        vec2_add(anchor, &self->config.offset, &pos);
        BulletConfig config;        
        config.speed = self->config.speed;
        config.angle = self->config.angle;
        f32 sa = (i - (self->config.count / 2)) * dynf32_get(&self->config.spread);
        sa += dynf32_get(&self->config.startAngle);
        config.baseAngle = sa;
        config.lifetime = dynf32_get(&self->config.lifetime);
        config.damage = (i32)dynf32_get(&self->config.damage);
        entity_create_bullet(entityManager, &config, &self->config.colliderConfig, pos, textures_get(self->config.textureName));
    }
}