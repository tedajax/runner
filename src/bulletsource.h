#ifndef RUNNER_BULLET_SOURCE_H
#define RUNNER_BULLET_SOURCE_H

#include "core.h"
#include "collider.h"

typedef struct bullet_source_t {
    BulletSourceConfig config;
    f32 fireTimer;
    u32 level;
    bool active;
} BulletSource;

typedef struct entity_manager_t EntityManager;

void bullet_source_init(BulletSource* self, char* config, char* section);
void bullet_source_config(BulletSourceConfig* self, char* config, char* section, u32 level);
void bullet_source_release(BulletSource* self);
inline void bullet_source_on(BulletSource* self) { self->active = true; }
inline void bullet_source_off(BulletSource* self) { self->active = false; }
void bullet_source_update(BulletSource* self, f32 dt, EntityManager* entityManager, Vec2* anchor);
void bullet_source_fire(BulletSource* self, EntityManager* entityManager, Vec2* anchor);

#endif