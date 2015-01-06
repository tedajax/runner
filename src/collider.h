#ifndef RUNNER_COLLIDER_H
#define RUNNER_COLLIDER_H

#include "core.h"
#include "physics.h"
#include "transformcomponent.h"
#include "config.h"

#define COLLIDER_MAX_COLLISIONS 32

typedef enum collider_layer_e {
    COLLIDER_LAYER_PLAYER,
    COLLIDER_LAYER_ENEMY,
    COLLIDER_LAYER_ENVIRONMENT,
    COLLIDER_LAYER_PLAYER_PROJECTILE,
    COLLIDER_LAYER_ENEMY_PROJECTILE,
    COLLIDER_LAYER_LAST,
} ColliderLayer;

typedef struct collider_t {
    i32 colliderId;
    ColliderLayer layer;
    BoundingVolume* volume;
    TransformComponent* anchor;
    Vec2 offset;
    i32 inContact[COLLIDER_MAX_COLLISIONS];
    u32 inContactCount;
} Collider;

void collider_init(Collider* self, ColliderLayer layer, TransformComponent* anchor, Vec2* offset);
void collider_init_aabb(Collider* self, ColliderLayer layer, TransformComponent* anchor, Vec2 offset, f32 width, f32 height);
void collider_init_obb(Collider* self, ColliderLayer layer, TransformComponent* anchor, Vec2 offset, f32 width, f32 height, f32 orientation);
void collider_init_bcircle(Collider* self, ColliderLayer layer, TransformComponent* anchor, Vec2 offset, f32 radius);
void collider_init_config(Collider* self, ColliderConfig* config, TransformComponent* anchor);
void collider_config(Collider* self, TransformComponent* anchor, char* config, char* section);
void collider_copy(const Collider* source, Collider* dest);
bool collider_on_screen(Collider* self);
bool collider_is_colliding(Collider* c1, Collider* c2);
bool collider_in_contact(Collider* self, Collider* other);
void collider_set_in_contact(Collider* c1, Collider* c2, bool inContact);
void collider_anchored_center(Collider* self, Vec2* dest);

ColliderLayer collider_layer_parse(char* str);

#endif