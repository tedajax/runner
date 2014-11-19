#ifndef RUNNER_GLOBALS_H
#define RUNNER_GLOBALS_H

#include "types.h"
#include "gametime.h"
#include "algebra.h"
#include "camera.h"

#include <SDL2/SDL.h>

// All globals should exist within the globals struct
// Try to limit total number of globals

typedef struct globals {
    u32 randomSeed;

    struct screen_t {
        u32 width;
        u32 height;
    } screen;

    struct world_t {
        u32 width;
        u32 height;
    } world;

    Vec2 gravity;

    GameTime time;

    SDL_Window* window;
    SDL_Renderer* renderer;

    Camera camera;

    //TODO texture management of some sort
    SDL_Texture* bulletTexture;
} Globals;

// Sets all the values in globals to reasonable defaults (0 most likely)
// Not intended to use as a configuration step.
void globals_init();

extern Globals globals;

#endif