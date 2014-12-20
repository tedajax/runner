#include "physics.h"

BoundingVolume* physics_volume_new(BoundingVolumeType type) {
    BoundingVolume* result = NULL;

    switch (type) {
        default:
        case BOUNDING_VOLUME_AA_BOX:
            result = (BoundingVolume*)CALLOC(1, AABoundingBox);
            break;

        case BOUNDING_VOLUME_O_BOX:
            result = (BoundingVolume*)CALLOC(1, OBoundingBox);
            break;

        case BOUNDING_VOLUME_CIRCLE:
            result = (BoundingVolume*)CALLOC(1, BoundingCircle);
            break;
    }

    return result;
}

bool physics_volumes_broadphase(BoundingVolume* bv1, BoundingVolume* bv2) {
    return rect_intersects(&bv1->bounds, &bv2->bounds);
}

bool physics_volumes_intersect(BoundingVolume* bv1, BoundingVolume* bv2) {
    switch (bv1->type) {
        default:
        case BOUNDING_VOLUME_AA_BOX:
            switch (bv2->type) {
                default:
                case BOUNDING_VOLUME_AA_BOX:
                    return aabbox_intersect_aabbox((AABoundingBox*)bv1, (AABoundingBox*)bv2);
                    break;

                case BOUNDING_VOLUME_O_BOX:
                    return aabbox_intersect_obbox((AABoundingBox*)bv1, (OBoundingBox*)bv2);
                    break;

                case BOUNDING_VOLUME_CIRCLE:
                    return aabbox_intersect_bcircle((AABoundingBox*)bv1, (BoundingCircle*)bv2);
                    break;
            }
            break;

        case BOUNDING_VOLUME_O_BOX:
            switch (bv2->type) {
                default:
                case BOUNDING_VOLUME_AA_BOX:
                    return obbox_intersect_aabbox((OBoundingBox*)bv1, (AABoundingBox*)bv2);
                    break;

                case BOUNDING_VOLUME_O_BOX:
                    return obbox_intersect_obbox((OBoundingBox*)bv1, (OBoundingBox*)bv2);
                    break;

                case BOUNDING_VOLUME_CIRCLE:
                    return obbox_intersect_bcircle((OBoundingBox*)bv1, (BoundingCircle*)bv2);
                    break;
            }
            break;

        case BOUNDING_VOLUME_CIRCLE:
            switch (bv2->type) {
                default:
                case BOUNDING_VOLUME_AA_BOX:
                    return bcircle_intersect_aabbox((BoundingCircle*)bv1, (AABoundingBox*)bv2);
                    break;

                case BOUNDING_VOLUME_O_BOX:
                    return bcircle_intersect_obbox((BoundingCircle*)bv1, (OBoundingBox*)bv2);
                    break;

                case BOUNDING_VOLUME_CIRCLE:
                    return bcircle_intersect_bcircle((BoundingCircle*)bv1, (BoundingCircle*)bv2);
                    break;
            }
            break;
    }
}

void physics_volume_update(BoundingVolume* self, Vec2* center, f32 orientation, Vec2* scale) {
    switch (self->type) {
        default:
        case BOUNDING_VOLUME_AA_BOX:
            aabbox_update((AABoundingBox*)self, center, orientation, scale);
            break;

        case BOUNDING_VOLUME_O_BOX:
            obbox_update((OBoundingBox*)self, center, orientation, scale);
            break;

        case BOUNDING_VOLUME_CIRCLE:
            bcircle_update((BoundingCircle*)self, center, orientation, scale);
            break;
    }
}

void bounding_volume_copy(BoundingVolume* source, BoundingVolume* dest) {
    dest->type = source->type;
    rect_copy_to(&source->bounds, &dest->bounds);

    switch (source->type) {
        default:
        case BOUNDING_VOLUME_AA_BOX:
            aabbox_copy((AABoundingBox*)source, (AABoundingBox*)dest);
            break;

        case BOUNDING_VOLUME_O_BOX:
            obbox_copy((OBoundingBox*)source, (OBoundingBox*)dest);
            break;

        case BOUNDING_VOLUME_CIRCLE:
            bcircle_copy((BoundingCircle*)source, (BoundingCircle*)dest);
            break;
    }
}

void aabbox_init(AABoundingBox* self, Vec2 center, f32 width, f32 height) {
    self->super.type = BOUNDING_VOLUME_AA_BOX;
    self->center = center;
    self->width = width;
    self->height = height;
    aabbox_calc_bounds(self);
}

void aabbox_calc_bounds(AABoundingBox* self) {
    self->super.bounds.position.x = self->center.x - self->width / 2.f;
    self->super.bounds.position.y = self->center.y - self->height / 2.f;
    self->super.bounds.width = self->width;
    self->super.bounds.height = self->height;
}

bool aabbox_intersect_aabbox(AABoundingBox* self, AABoundingBox* other) {
    return rect_intersects(&self->super.bounds, &other->super.bounds);
}

bool aabbox_intersect_obbox(AABoundingBox* self, OBoundingBox* other) {
    return obbox_intersect_aabbox(other, self);
}

bool aabbox_intersect_bcircle(AABoundingBox* self, BoundingCircle* other) {
    f32 w = self->width / 2.f + other->radius * 2.f;
    f32 h = self->height / 2.f + other->radius * 2.f;
    
    return other->center.x >= self->center.x - w &&
           other->center.x <= self->center.x + w &&
           other->center.y >= self->center.y - h &&
           other->center.y <= self->center.y + h;
}

void aabbox_copy(AABoundingBox* source, AABoundingBox* dest) {
    vec2_copy_to(&source->center, &dest->center);
    dest->width = source->width;
    dest->height = source->height;
}

void aabbox_update(AABoundingBox* self, Vec2* center, f32 rotation, Vec2* scale) {
    vec2_copy_to(center, &self->center);
    aabbox_calc_bounds(self);
}

void obbox_init(OBoundingBox* self, Vec2 center, f32 width, f32 height, f32 orientation) {
    self->super.type = BOUNDING_VOLUME_O_BOX;
    self->center = center;
    self->width = width;
    self->height = height;
    self->orientation = orientation;
    obbox_calc_corners(self);
    obbox_calc_axes(self);
    obbox_calc_bounds(self);
}

void obbox_calc_bounds(OBoundingBox* self) {
    f32 xMin = FLT_MAX;
    f32 yMin = FLT_MAX;
    f32 xMax = -FLT_MAX;
    f32 yMax = -FLT_MAX;

    for (u32 c = 0; c < 4; ++c) {
        Vec2 corner = self->corners[c];

        if (corner.x < xMin) {
            xMin = corner.x;
        }
        if (corner.x > xMax) {
            xMax = corner.x;
        }
        if (corner.y < yMin) {
            yMin = corner.y;
        }
        if (corner.y > yMax) {
            yMax = corner.y;
        }
    }

    self->super.bounds.position.x = xMin;
    self->super.bounds.position.y = yMin;
    self->super.bounds.width = xMax - xMin;
    self->super.bounds.height = yMax - yMin;
}

void obbox_calc_corners(OBoundingBox* self) {
    f32 a = self->orientation * DEG_TO_RAD;
    Vec2 x = vec2_init(cosf(a), sinf(a));
    Vec2 y = vec2_init(-sinf(a), cosf(a));

    vec2_scale(&x, self->width / 2.f, &x);
    vec2_scale(&y, self->height / 2.f, &y);

    for (u32 i = 0; i < 4; ++i) {
        vec2_copy_to(&self->center, &self->corners[i]);
    }

    vec2_sub(&self->corners[0], &x, &self->corners[0]);
    vec2_sub(&self->corners[0], &y, &self->corners[0]);

    vec2_add(&self->corners[1], &x, &self->corners[1]);
    vec2_sub(&self->corners[1], &y, &self->corners[1]);

    vec2_add(&self->corners[2], &x, &self->corners[2]);
    vec2_add(&self->corners[2], &y, &self->corners[2]);

    vec2_sub(&self->corners[3], &x, &self->corners[3]);
    vec2_add(&self->corners[3], &y, &self->corners[3]);
}

void obbox_calc_axes(OBoundingBox* self) {
    vec2_sub(&self->corners[1], &self->corners[0], &self->axes[0]);
    vec2_sub(&self->corners[3], &self->corners[0], &self->axes[1]);

    for (u32 a = 0; a < 2; ++a) {
        f32 axisSqrLen = vec2_lengthSqr(&self->axes[a]);
        vec2_scale(&self->axes[a], 1.f / axisSqrLen, &self->axes[a]);
        self->origins[a] = vec2_dot(&self->corners[0], &self->axes[a]);
    }
}

bool obbox_overlaps_axis(OBoundingBox* self, OBoundingBox* other) {
    for (u32 a = 0; a < 2; ++a) {
        f32 t = vec2_dot(&other->corners[0], &self->axes[a]);

        f32 tMin = t;
        f32 tMax = t;

        for (u32 c = 1; c < 4; ++c) {
            t = vec2_dot(&other->corners[c], &self->axes[a]);

            if (t < tMin) {
                tMin = t;
            } else if (t > tMax) {
                tMax = t;
            }
        }

        if ((tMin > 1.f + self->origins[a]) || (tMax < self->origins[a])) {
            return false;
        }
    }

    return true;
}

bool obbox_intersect_aabbox(OBoundingBox* self, AABoundingBox* other) {
    // Kind of stupid inefficient way to do this but it's easy
    OBoundingBox temp;
    obbox_init(&temp, other->center, other->width, other->height, 0.f);
    return obbox_intersect_obbox(self, &temp);
}

bool obbox_intersect_obbox(OBoundingBox* self, OBoundingBox* other) {
    return obbox_overlaps_axis(self, other) && obbox_overlaps_axis(other, self);
}

bool obbox_intersect_bcircle(OBoundingBox* self, BoundingCircle* other) {
    // TODO
    return false;
}

void obbox_copy(OBoundingBox* source, OBoundingBox* dest) {
    vec2_copy_to(&source->center, &dest->center);
    dest->width = source->width;
    dest->height = source->height;
    dest->orientation = source->orientation;

    for (u32 c = 0; c < 4; ++c) {
        vec2_copy_to(&source->corners[c], &dest->corners[c]);
    }

    for (u32 a = 0; a < 2; ++a) {
        vec2_copy_to(&source->axes[a], &dest->axes[a]);
    }

    dest->origins[0] = source->origins[0];
    dest->origins[1] = source->origins[1];
}

void obbox_update(OBoundingBox* self, Vec2* center, f32 rotation, Vec2* scale){
    vec2_copy_to(center, &self->center);
    self->orientation = rotation;
    obbox_calc_corners(self);
    obbox_calc_axes(self);
    obbox_calc_bounds(self);
}

void bcircle_init(BoundingCircle* self, Vec2 center, f32 radius) {
    self->super.type = BOUNDING_VOLUME_CIRCLE;
    self->center = center;
    self->radius = radius;
    bcircle_calc_bounds(self);
}

void bcircle_calc_bounds(BoundingCircle* self) {
    self->super.bounds.position.x = self->center.x - self->radius;
    self->super.bounds.position.y = self->center.y - self->radius;
    self->super.bounds.width = self->radius * 2.f;
    self->super.bounds.height = self->radius * 2.f;
}

bool bcircle_intersect_aabbox(BoundingCircle* self, AABoundingBox* other) {
    return aabbox_intersect_bcircle(other, self);
}

bool bcircle_intersect_obbox(BoundingCircle* self, OBoundingBox* other) {
    return obbox_intersect_bcircle(other, self);
}

bool bcircle_intersect_bcircle(BoundingCircle* self, BoundingCircle* other) {
    f32 d = vec2_dist(&self->center, &other->center);
    return d <= self->radius + other->radius;
}

void bcircle_copy(BoundingCircle* source, BoundingCircle* dest) {
    vec2_copy_to(&source->center, &dest->center);
    dest->radius = source->radius;
}

void bcircle_update(BoundingCircle* self, Vec2* center, f32 rotation, Vec2* scale) {
    vec2_copy_to(center, &self->center);
    bcircle_calc_bounds(self);
}