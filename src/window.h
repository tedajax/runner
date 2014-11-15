#ifndef RUNNER_WINDOW_H
#define RUNNER_WINDOW_H

#include <SDL2/SDL.h>

#include "types.h"
#include "log.h"

SDL_Window* window_create(const char* title,
    int positionX,
    int positionY,
    int width,
    int height,
    u32 flags);

#endif