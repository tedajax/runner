#ifndef RUNNER_BULLET_SOURCE_H
#define RUNNER_BULLET_SOURCE_H

#include "core.h"
#include "collider.h"

typedef struct bullet_source_t {
    BulletSourceConfig* config;
    f32 fireTimer;
    u32 level;
    bool active;
} BulletSource;

typedef struct entity_manager_t EntityManager;

void bullet_source_init(BulletSource* self, BulletSourceConfig* config);
void bullet_source_release(BulletSource* self);
static inline void bullet_source_on(BulletSource* self) { self->active = true; }
static inline void bullet_source_off(BulletSource* self) { self->active = false; }
void bullet_source_update(BulletSource* self, f32 dt, EntityManager* entityManager, Vec2* anchor);
void bullet_source_fire(BulletSource* self, EntityManager* entityManager, Vec2* anchor);

#endif