#include "bulletsource.h"
#include "entitymanager.h"
#include "entityfactory.h"
#include "texturemanager.h"

void bullet_source_init(BulletSource* self, BulletSourceConfig* config) {
    self->active = false;
    self->fireTimer = 0.f;
    self->burstTimer = 0.f;
    self->burstShotsRemaining = 0;
    self->level = 0;
    self->config = config;
}

void bullet_source_release(BulletSource* self) {
    dynf32_release(&self->config->burstDelay);
    dynf32_release(&self->config->spread);
    dynf32_release(&self->config->lifetime);
    dynf32_release(&self->config->speed);
    dynf32_release(&self->config->angle);
    dynf32_release(&self->config->fireDelay);
    dynf32_release(&self->config->startAngle);
    dynf32_release(&self->config->damage);
}

void bullet_source_update(BulletSource* self, f32 dt, EntityManager* entityManager, Vec2* anchor) {
    if (self->burstShotsRemaining > 0) {
        if (!self->active) {
            self->burstShotsRemaining = 0;
        }

        if (self->burstTimer > 0.f) {
            self->burstTimer -= dt;
        }

        if (self->burstTimer <= 0.f) {
            bullet_source_fire(self, entityManager, anchor);
            --self->burstShotsRemaining;
            if (self->burstShotsRemaining > 0) {
                self->burstTimer = dynf32_get(&self->config->burstDelay);
            }
        }
    } else {
        if (self->fireTimer > 0.f) {
            self->fireTimer -= dt;
        }

        if (!self->active) {
            return;
        }

        if (self->fireTimer <= 0.f) {
            self->fireTimer = dynf32_get(&self->config->fireDelay);
            self->burstShotsRemaining = self->config->burstCount;
        }
    }
}

void bullet_source_fire(BulletSource* self, EntityManager* entityManager, Vec2* anchor) {
    for (i32 i = 0; i < self->config->count; ++i) {
        Vec2 pos;
        vec2_add(anchor, &self->config->offset, &pos);
        BulletConfig config;        
        config.speed = self->config->speed;
        config.angle = self->config->angle;
        f32 sa = (i - (self->config->count / 2)) * dynf32_get(&self->config->spread);
        sa += dynf32_get(&self->config->startAngle);
        config.baseAngle = sa;
        config.lifetime = dynf32_get(&self->config->lifetime);
        config.damage = (i32)dynf32_get(&self->config->damage);
        entity_create_bullet(entityManager, &config, self->config->colliderConfig, pos, textures_get(self->config->textureName));
    }
}