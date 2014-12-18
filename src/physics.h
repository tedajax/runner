#ifndef RUNNER_PHYSICS_H
#define RUNNER_PHYSICS_H

#include "core.h"

typedef enum bounding_volume_type_e {
    BOUNDING_VOLUME_AA_BOX,
    BOUNDING_VOLUME_O_BOX,
    BOUNDING_VOLUME_CIRCLE,
    BOUNDIGN_VOLUME_LAST,
} BoundingVolumeType;

typedef struct bounding_volume_t {
    BoundingVolumeType type;
    Rect bounds;    
} BoundingVolume;

typedef struct aa_bounding_box_t {
    BoundingVolume super;
    Vec2 center;
    f32 width;
    f32 height;
} AABoundingBox;

typedef struct o_bounding_box_t {
    BoundingVolume super;
    Vec2 center;
    f32 width;
    f32 height;
    f32 orientation;

    Vec2 corners[4];
    // edges originating from corner[0]
    Vec2 axes[2];
    f32 origins[2];
} OBoundingBox;

typedef struct bounding_circle_t {
    BoundingVolume super;
    Vec2 center;
    f32 radius;
} BoundingCircle;

BoundingVolume* physics_volume_new(BoundingVolumeType type);
bool physics_volumes_broadphase(BoundingVolume* bv1, BoundingVolume* bv2);
bool physics_volumes_intersect(BoundingVolume* bv1, BoundingVolume* bv2);
void physics_volume_update(BoundingVolume* self, Vec2* center);

void bounding_volume_copy(BoundingVolume* source, BoundingVolume* dest);

void aabbox_init(AABoundingBox* self, Vec2 center, f32 width, f32 height);
void aabbox_calc_bounds(AABoundingBox* self);
bool aabbox_intersect_aabbox(AABoundingBox* self, AABoundingBox* other);
bool aabbox_intersect_obbox(AABoundingBox* self, OBoundingBox* other);
bool aabbox_intersect_bcircle(AABoundingBox* self, BoundingCircle* other);
void aabbox_copy(AABoundingBox* source, AABoundingBox* dest);
void aabbox_update(AABoundingBox* self, Vec2* center);

void obbox_init(OBoundingBox* self, Vec2 center, f32 width, f32 height, f32 orientation);
void obbox_calc_bounds(OBoundingBox* self);
void obbox_calc_corners(OBoundingBox* self);
void obbox_calc_axes(OBoundingBox* self);
bool obbox_overlaps_axis(OBoundingBox* self, OBoundingBox* other);
bool obbox_intersect_aabbox(OBoundingBox* self, AABoundingBox* other);
bool obbox_intersect_obbox(OBoundingBox* self, OBoundingBox* other);
bool obbox_intersect_bcircle(OBoundingBox* self, BoundingCircle* other);
void obbox_copy(OBoundingBox* source, OBoundingBox* dest);
void obbox_update(OBoundingBox* self, Vec2* center);

void bcircle_init(BoundingCircle* self, Vec2 center, f32 radius);
void bcircle_calc_bounds(BoundingCircle* self);
bool bcircle_intersect_aabbox(BoundingCircle* self, AABoundingBox* other);
bool bcircle_intersect_obbox(BoundingCircle* self, OBoundingBox* other);
bool bcircle_intersect_bcircle(BoundingCircle* self, BoundingCircle* other);
void bcircle_copy(BoundingCircle* source, BoundingCircle* dest);
void bcircle_update(BoundingCircle* self, Vec2* center);

#endif