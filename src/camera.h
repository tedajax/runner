#ifndef RUNNER_CAMERA_H
#define RUNNER_CAMERA_H

#include "types.h"
#include "algebra.h"

#include <SDL2/SDL.h>

typedef struct camera_t {
    Vec2 position;
    Vec2* target;

    //If the target moves outside of these constraints the camera will
    //move to follow it.  This rectangle is relative to the position of the camera.
    SDL_Rect constraints;

    Rect cameraView;
    Rect worldView;
} Camera;

void camera_init(Camera* self, Vec2* target, SDL_Rect* constraints);
void camera_contrain(Camera* self, Vec2* point);
void camera_update(Camera* self);

#endif