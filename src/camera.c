#include "camera.h"
#include "globals.h"

void camera_init(Camera* self, Vec2* target, SDL_Rect* constraints) {
    self->position = vec2_zero();

    self->target = target;

    if (constraints) {
        self->constraints.x = constraints->x;
        self->constraints.y = constraints->y;
        self->constraints.w = constraints->w;
        self->constraints.h = constraints->h;
    }

    rect_set(&self->cameraView, &self->position,
        (f32)globals.world.width,
        (f32)globals.world.height);
}

void camera_contrain(Camera* self, Vec2* point) {
    f32 left = self->position.x + self->constraints.x;
    f32 right = self->position.x + self->constraints.x + self->constraints.w;
    f32 top = self->position.y + self->constraints.y;
    f32 bottom = self->position.y + self->constraints.y + self->constraints.h;

    if (point->x < left) { point->x = left; }
    else if (point->x > right) { point->x = right; }

    if (point->y < top) { point->y = top; }
    else if (point->y > bottom) { point->y = bottom; }
}

void camera_update(Camera* self) {
    /*if (!self->target) {
        return;
    }

    f32 left = self->position.x + self->constraints.x;
    f32 right = self->position.x + self->constraints.x + self->constraints.w;
    f32 top = self->position.y + self->constraints.y;
    f32 bottom = self->position.y + self->constraints.y + self->constraints.h;

    if (self->target->x < left) {
        self->position.x = self->target->x - self->constraints.x;
    } else if (self->target->x > right) {
        self->position.x = self->target->x - (self->constraints.x + self->constraints.w);
    }

    if (self->target->y < top) {
        self->position.y = self->target->y - self->constraints.y;
    } else if (self->target->y > bottom) {
        self->position.y = self->target->y - (self->constraints.y + self->constraints.h);
    }*/

    self->position.x += globals.scrollSpeed * globals.time.delta;

    rect_set(&self->worldView, &self->position,
        (f32)globals.world.width,
        (f32)globals.world.height);
}