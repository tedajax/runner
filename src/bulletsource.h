#ifndef RUNNER_BULLET_SOURCE_H
#define RUNNER_BULLET_SOURCE_H

#include "core.h"
#include "collider.h"

typedef struct bullet_source_t {
    BulletSourceConfig* config;
    f32 fireTimer;
    f32 burstTimer;
    i32 burstsRemaining;
    i32 burstShotsRemaining;
    u32 level;
    bool active;
} BulletSource;

typedef struct entity_manager_t EntityManager;

void bullet_source_init(BulletSource* self, BulletSourceConfig* config);
void bullet_source_release(BulletSource* self);

static inline void bullet_source_on(BulletSource* self) { 
    self->active = true;
    self->fireTimer = 0.f;
    self->burstsRemaining = self->config->burstCount;
}

static inline void bullet_source_off(BulletSource* self) {
    self->active = false;
    self->fireTimer = 0.f;
}

void bullet_source_update(BulletSource* self, f32 dt, EntityManager* entityManager, Vec2* anchor);
void bullet_source_fire(BulletSource* self, EntityManager* entityManager, Vec2* anchor);

#endif