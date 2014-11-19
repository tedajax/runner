#include "camera.h"

void camera_init(Camera* self, Vec2* target, SDL_Rect* constraints) {
    self->position = vec2_zero();

    self->target = target;

    if (constraints) {
        self->constraints.x = constraints->x;
        self->constraints.y = constraints->y;
        self->constraints.w = constraints->w;
        self->constraints.h = constraints->h;
    }
}

void camera_update(Camera* self) {
    if (!self->target) {
        return;
    }

    f32 left = self->position.x + self->constraints.x;
    f32 right = self->position.x + self->constraints.x + self->constraints.w;
    f32 top = self->position.y + self->constraints.y;
    f32 bottom = self->position.y + self->constraints.y + self->constraints.h;

    if (self->target->x < left) {
        self->position.x = self->target->x;
    } else if (self->target->x > right) {
        self->position.x = self->target->x - (self->target->x - self->position.x);
    }

    if (self->target->y < top) {
        self->position.y = self->target->y + top;
    } else if (self->target->y > bottom) {
        self->position.y = self->target->y - bottom;
    }
}