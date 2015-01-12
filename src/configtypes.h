#ifndef RUNNER_CONFIG_TYPES_H
#define RUNNER_CONFIG_TYPES_H

#include "types.h"
#include "algebra.h"

#include "dynf32.h"

typedef enum type_config_type_e {
    TYPE_CONFIG_COLLIDER = 0,
    TYPE_CONFIG_BULLET_SOURCE,
    TYPE_CONFIG_TWEEN,
    TYPE_CONFIG_LAST
} TypeConfigType;

typedef struct type_config_t {
    TypeConfigType type;
} TypeConfig;

typedef struct config_t Config;
typedef void(*deserialize_type_f)(TypeConfig*, Config*, char*);

deserialize_type_f deserializeFunctions[TYPE_CONFIG_LAST];

#define REGISTER_DESERIALIZE_FUNCTION(type, func) \
    deserializeFunctions[type] = func;

#define DESERIALIZE(type, typeconfig, config, table) \
    deserializeFunctions[type](typeconfig, config, table);

typedef enum collider_layer_e ColliderLayer;
typedef enum bounding_volume_type_e BoundingVolumeType;

typedef struct collider_config_t {
    TypeConfig super;
    i32 layer;
    Vec2 offset;
    i32 type;
    f32 width;
    f32 height;
    f32 radius;
    f32 orientation;
} ColliderConfig;

void collider_config_deserialize(TypeConfig* super, Config* config, char* table);
ColliderLayer collider_layer_parse(char* str);

typedef struct bullet_source_config_t {
    TypeConfig super;
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
    ColliderConfig* colliderConfig;
} BulletSourceConfig;

void bullet_source_config_deserialize(TypeConfig* super, Config* config, char* table);

typedef f32(*tween_func)(f32, f32, f32, f32);

typedef struct tween_config_t {
    TypeConfig super;
    f32 start;
    f32 end;
    f32 duration;
    f32 timescale;
    u32 loops;
    tween_func function;
} TweenConfig;

void tween_config_deserialize(TypeConfig* super, Config* config, char* table);

#endif