#include "application.h"

int app_run(int argc, char* argv[]) {
    ASSERT(app_initialize(), "Application failed to initialize");

    SDL_SetRenderDrawColor(globals.renderer, 0, 0, 0, 255);
    SDL_RenderClear(globals.renderer);
    SDL_RenderPresent(globals.renderer);

    SDL_Delay(2000);

    app_terminate();

    return 0;
}

bool app_initialize() {
    globals.randomSeed = time(NULL);
    srand(globals.randomSeed);

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        log_error("SDL", SDL_GetError());
        return false;
    }

    globals.screen.width = 800;
    globals.screen.height = 600;

    globals.window = window_create("Runner",
        0,
        16,
        globals.screen.width,
        globals.screen.height,
        SDL_WINDOW_SHOWN);

    if (!globals.window) {
        return false;
    }

    globals.renderer = SDL_CreateRenderer(globals.window, -1, SDL_RENDERER_ACCELERATED);
    if (!globals.renderer) {
        log_error("SDL", SDL_GetError());
        return false;
    }

    return true;
}

void app_terminate() {
    SDL_DestroyWindow(globals.window);

    SDL_Quit();
}