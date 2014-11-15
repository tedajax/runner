#include "application.h"

int app_run(int argc, char* argv[]) {
    ASSERT(app_initialize(), "Application failed to initialize");

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

    return true;
}

void app_terminate() {
    SDL_Quit();
}