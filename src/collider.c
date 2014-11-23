#include "collider.h"

void collider_init(Collider* self, ColliderLayer layer, Vec2* anchor) {
    self->colliderId = -1;
    self->layer = layer;
    self->anchor = anchor;
    self->inContactCount = 0;

    for (u32 i = 0; i < COLLIDER_MAX_COLLISIONS; ++i) {
        self->inContact[i] = -1;
    }
}

void collider_init_circle(Collider* self, ColliderLayer layer, Vec2 offset, f32 radius, Vec2* anchor) {
    collider_init(self, layer, anchor);
    self->shape = COLLIDER_SHAPE_CIRCLE;
    circle_set(&self->circle, &offset, radius);
}

void collider_init_rectangle(Collider* self, ColliderLayer layer, Vec2 offset, f32 width, f32 height, Vec2* anchor) {
    collider_init(self, layer, anchor);
    self->shape = COLLIDER_SHAPE_RECTANGLE;
    rect_set(&self->rectangle, &offset, width, height);
}

void collider_copy(const Collider* source, Collider* dest) {
    dest->colliderId = source->colliderId;
    dest->layer = source->layer;
    dest->shape = source->shape;
    circle_copy_to(&source->circle, &dest->circle);
    rect_copy_to(&source->rectangle, &dest->rectangle);
    dest->anchor = source->anchor;
    dest->inContactCount = source->inContactCount;
    for (u32 i = 0; i < dest->inContactCount; ++i) {
        dest->inContact[i] = source->inContact[i];
    }
}

bool collider_is_colliding(Collider* c1, Collider* c2) {
    switch (c1->shape) {
        case COLLIDER_SHAPE_CIRCLE: {
            Circle shape1;
            collider_anchored_circle(c1, &shape1);
            switch (c2->shape) {
                case COLLIDER_SHAPE_CIRCLE: {
                    Circle shape2;
                    collider_anchored_circle(c2, &shape2);
                    return circle_intersects(&shape1, &shape2);
                }

                case COLLIDER_SHAPE_RECTANGLE: {
                    Rect shape2;
                    collider_anchored_rectangle(c2, &shape2);
                    return rect_intersects_circle(&shape2, &shape1);
                }

                default:
                    return false;
            }
            break;
        }

        case COLLIDER_SHAPE_RECTANGLE: {
            Rect shape1;
            collider_anchored_rectangle(c1, &shape1);
            switch (c2->shape) {
                case COLLIDER_SHAPE_CIRCLE: {
                    Circle shape2;
                    collider_anchored_circle(c2, &shape2);
                    return rect_intersects_circle(&shape1, &shape2);
                }

                case COLLIDER_SHAPE_RECTANGLE: {
                    DEBUG_ON_KEY(SDL_SCANCODE_L);
                    Rect shape2;
                    collider_anchored_rectangle(c2, &shape2);
                    return rect_intersects(&shape1, &shape2);
                }

                default:
                    return false;
            }
            break;
        }

        default:
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

void collider_anchored_circle(Collider* self, Circle* dest) {
    vec2_set(&dest->position,
        self->circle.position.x + self->anchor->x,
        self->circle.position.y + self->anchor->y);
    dest->radius = self->circle.radius;
}

void collider_anchored_rectangle(Collider* self, Rect* dest) {
    vec2_set(&dest->position,
        self->rectangle.position.x + self->anchor->x,
        self->rectangle.position.y + self->anchor->y);
    dest->width = self->rectangle.width;
    dest->height = self->rectangle.height;
}