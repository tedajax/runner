#ifndef RUNNER_BULLET_SOURCE_H
#define RUNNER_BULLET_SOURCE_H

#include "core.h"
#include "collider.h"

typedef struct bullet_source_config_t {
	Vec2 offset;		// offset from parent
    i32 count;          // how many bullets to fire from this source
    dynf32 spread;         // the spread angle of this source
    dynf32 lifetime;       // how long the bullet lives before they are destroyed
    dynf32 speed;          // how fast the bullet moves
    dynf32 angle;          // individual bullet angle adjustment (should be 0 unless it's dynamic)
    dynf32 fireDelay;      // Time between shots
    dynf32 startAngle;     // Direction to fire
    dynf32 damage;         // Damage done by bullet
    char* textureName;  // bullet texture
	ColliderConfig colliderConfig;
} BulletSourceConfig;

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