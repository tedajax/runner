#include "collider.h"

void collider_init_circle(Collider* self, ColliderLayer layer, Vec2 offset, f32 radius, Vec2* anchor) {
    self->colliderId = -1;
    
    self->layer = layer;
    self->shape = COLLIDER_SHAPE_CIRCLE;
    circle_set(&self->circle, &offset, radius);

    self->anchor = anchor;

    self->inContactCount = 0;
}

void collider_init_rectangle(Collider* self, ColliderLayer layer, Vec2 offset, f32 width, f32 height, Vec2* anchor) {
    self->colliderId = -1;

    self->layer = layer;
    self->shape = COLLIDER_SHAPE_RECTANGLE;
    rect_set(&self->rectangle, &offset, width, height);

    self->anchor = anchor;

    self->inContactCount = 0;
}

void collider_copy(const Collider* source, Collider* dest) {
    dest->colliderId = source->colliderId;
    dest->layer = source->layer;
    dest->shape = source->shape;
    circle_copy_to(&source->circle, &dest->circle);
    rect_copy_to(&source->rectangle, &dest->rectangle);
    dest->anchor = source->anchor;
    dest->inContactCount = source->inContactCount;
    for (size_t i = 0; i < dest->inContactCount; ++i) {
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