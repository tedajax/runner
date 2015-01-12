#include "collider.h"

void collider_init(Collider* self, ColliderLayer layer, TransformComponent* anchor, Vec2* offset) {
    self->colliderId = -1;
    self->layer = layer;
    self->anchor = anchor;
    vec2_copy_to(offset, &self->offset);
    self->inContactCount = 0;

    self->volume = NULL;

    for (u32 i = 0; i < COLLIDER_MAX_COLLISIONS; ++i) {
        self->inContact[i] = -1;
    }
}

void collider_init_aabb(Collider* self, ColliderLayer layer, TransformComponent* anchor, Vec2 offset, f32 width, f32 height) {
    collider_init(self, layer, anchor, &offset);

    self->volume = (BoundingVolume*)CALLOC(1, AABoundingBox);

    Vec2 position;
    collider_anchored_center(self, &position);
    aabbox_init((AABoundingBox*)self->volume, position, width, height);
}

void collider_init_obb(Collider* self, ColliderLayer layer, TransformComponent* anchor, Vec2 offset, f32 width, f32 height, f32 orientation) {
    collider_init(self, layer, anchor, &offset);

    self->volume = (BoundingVolume*)CALLOC(1, OBoundingBox);

    Vec2 position;
    collider_anchored_center(self, &position);
    obbox_init((OBoundingBox*)self->volume, position, width, height, orientation);
}

void collider_init_bcircle(Collider* self, ColliderLayer layer, TransformComponent* anchor, Vec2 offset, f32 radius) {
    collider_init(self, layer, anchor, &offset);

    self->volume = (BoundingVolume*)CALLOC(1, BoundingCircle);

    Vec2 position;
    collider_anchored_center(self, &position);
    bcircle_init((BoundingCircle*)self->volume, position, radius);
}

void collider_init_config(Collider* self, ColliderConfig* config, TransformComponent* anchor) {
    switch (config->type) {
        default:
        case BOUNDING_VOLUME_AA_BOX:
            collider_init_aabb(self, config->layer, anchor, config->offset, config->width, config->height);
            break;

        case BOUNDING_VOLUME_O_BOX:
            collider_init_obb(self, config->layer, anchor, config->offset, config->width, config->height, config->orientation);
            break;

        case BOUNDING_VOLUME_CIRCLE:
            collider_init_bcircle(self, config->layer, anchor, config->offset, config->radius);
            break;
    }
}

void collider_copy(const Collider* source, Collider* dest) {
    dest->colliderId = source->colliderId;
    dest->layer = source->layer;
    
    vec2_copy_to(&source->offset, &dest->offset);
    dest->volume = physics_volume_new(source->volume->type);
    bounding_volume_copy(source->volume, dest->volume);
    dest->anchor = source->anchor;
    dest->inContactCount = source->inContactCount;
    for (u32 i = 0; i < dest->inContactCount; ++i) {
        dest->inContact[i] = source->inContact[i];
    }
}

bool collider_on_screen(Collider* self) {
    return rect_intersects(&self->volume->bounds, &globals.camera.worldView);
}

bool collider_is_colliding(Collider* c1, Collider* c2) {
    bool broadphase = physics_volumes_broadphase(c1->volume, c2->volume);
        
    if (broadphase) {
        return physics_volumes_intersect(c1->volume, c2->volume);
    } else {
        return false;
    }
}

bool collider_in_contact(Collider* self, Collider* other) {
    for (u32 i = 0; i < self->inContactCount; ++i) {
        if (self->inContact[i] == other->colliderId) {
            return true;
        }
    }

    return false;
}

void collider_set_in_contact(Collider* c1, Collider* c2, bool inContact) {
    if (inContact) {
        ASSERT(c1->inContactCount < COLLIDER_MAX_COLLISIONS, "Max collision count reached.");
        ASSERT(c2->inContactCount < COLLIDER_MAX_COLLISIONS, "Max collision count reached.");

        c1->inContact[c1->inContactCount] = c2->colliderId;
        c2->inContact[c2->inContactCount] = c1->colliderId;

        ++c1->inContactCount;
        ++c2->inContactCount;
    } else {
        if (c1->inContactCount > 0) {
            for (u32 i = 0; i < c1->inContactCount; ++i) {
                if (c1->inContact[i] == c2->colliderId) {
                    //remove by overwriting
                    for (u32 j = i + 1; j < c1->inContactCount; ++j) {
                        c1->inContact[j - 1] = c1->inContact[j];
                    }
                    --c1->inContactCount;
                }
            }
        }

        if (c2->inContactCount > 0) {
            for (u32 i = 0; i < c2->inContactCount; ++i) {
                if (c2->inContact[i] == c1->colliderId) {
                    //remove by overwriting
                    for (u32 j = i + 1; j < c2->inContactCount; ++j) {
                        c2->inContact[j - 1] = c2->inContact[j];
                    }
                    --c2->inContactCount;
                }
            }
        }
    }
}

void collider_anchored_center(Collider* self, Vec2* dest) {
    dest->x = self->anchor->position.x + self->offset.x;
    dest->y = self->anchor->position.y + self->offset.y;
}
