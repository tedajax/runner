#ifndef RUNNER_BULLET_SOURCE_H
#define RUNNER_BULLET_SOURCE_H

#include "core.h"

typedef struct bullet_source_config_t {
    u32 count;          // how many bullets to fire from this source
    f32 spread;         // the spread angle of this source
    f32 lifetime;       // how long the bullet lives before they are destroyed
    f32 speed;          // how fast the bullet moves
    f32 acceleration;   // how the speed of the bullet changes over its lifetime
    f32 fireDelay;      // Time between shots
    f32 startAngle;     // Direction to fir
    f32 rotationRate;   // rate of rotation while in flight
    char* textureName;  // bullet texture
} BulletSourceConfig;

typedef struct bullet_source_t {
    BulletSourceConfig config;
    f32 fireTimer;
    Vec2 offset;
    u32 level;
    bool active;
} BulletSource;

typedef struct entity_manager_t EntityManager;

void bullet_source_init(BulletSource* self, Vec2* offset, char* config, char* section);
void bullet_source_config(BulletSourceConfig* self, char* config, char* section, u32 level);
bool bullet_source_update(BulletSource* self, f32 dt);
void bullet_source_fire(BulletSource* self, EntityManager* entityManager, Vec2* anchor);

#endif